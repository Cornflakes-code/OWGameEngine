#include "NoMansSkyStarMap.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <regex>
#include <random>
#include <cmath>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <json/single_include/nlohmann/json.hpp>


#include <Core/CommonUtils.h>
#include <Core/ErrorHandling.h>
#include <Core/GlobalSettings.h>

#include <Helpers/GeometricShapes.h>
#include <Helpers/Shader.h>
#include <Helpers/TextData.h>

#include <Renderers/InstanceRenderer.h>
#include <Renderers/TextRendererStatic.h>

NoMansSky::NoMansSky()
	: mGridShader(new Shader("Lines.v.glsl", "Lines.f.glsl", ""))
{
}

#define DEBUG_GRID
#define DEBUG_STARS

void NoMansSky::setUp(const std::string& fileName, const AABB& world)
{
	glm::u32vec3 gridSizes({ 0xAA, 0xAA, 0xAA });
	glGenVertexArrays(2, &mVao[0]);
	AABB NMSSize(glm::vec4(-0x7FF, -0x7F, -0x7FF, 1),
				 glm::vec4(0x7FF, 0x7F, 0x7FF, 1));
	float scaleNMStoWorld = world.size().x / NMSSize.size().x;
#ifdef DEBUG_GRID
	createGrid(NMSSize, gridSizes, scaleNMStoWorld);
	glBindVertexArray(mVao[0]);

	GLuint vboGrid;
	glGenBuffers(1, &vboGrid);
	glBindBuffer(GL_ARRAY_BUFFER, vboGrid);
	mGridShader->use();
	GLuint location = mGridShader->getAttributeLocation("vPos");
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mGrid.size(), 
					mGrid.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glLineWidth(1.0f);
	glEnableVertexAttribArray(location);
	glBindVertexArray(0);
#endif

#ifdef DEBUG_STARS
	loadStars(fileName, NMSSize, scaleNMStoWorld);
	MeshDataInstance mdi;
	std::vector<glm::vec3> squareVertices
		= GeometricShapes::rectangle(glm::vec2(1.0, 1.0), glm::vec2(-0.5, -0.5));
	mdi.vertices(squareVertices, GL_TRIANGLES, 0);
	
	mRandomMinorStars = createRandomVectors(NMSSize, 50000, scaleNMStoWorld);
	mdi.positions(mRandomMinorStars, 1, 1);

	std::vector<glm::vec4> instanceColours;
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::YELLOW));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::GREEN));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::RED));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::BRIGHT_BLUE));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::BRIGHT_MAGENTA));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::CYAN));
	mdi.colours(instanceColours, 1, 2);
//#define FANCY_SHADER
#ifdef FANCY_SHADER
		Shader* instanceShader = new Shader("thebookofshaders.v.glsl",
			"solarSuns.f.glsl",
			"thebookofshaders_circle.g.glsl");
#else
	Shader* instanceShader = new Shader("instanced.v.glsl",
		"instanced_snoise.f.glsl",
		//"instanced.f.glsl",
//					"instanced.g.glsl"
""
);
#endif
	mStarRenderer = new InstanceRenderer(instanceShader, "VP");
	mStarRenderer->setup(&mdi);
#endif
}

void NoMansSky::createGrid(const AABB& nmsSpace,
						   const glm::u32vec3& gridSizes,
						   float scaleToWorld)
{
	// https://nomanssky.gamepedia.com/Portal_address

	for (glm::vec4::value_type x = nmsSpace.minPoint().x;
			x <= nmsSpace.maxPoint().x; x += gridSizes.x)
	{
		const float xf = x * scaleToWorld;
		for (glm::vec4::value_type y = nmsSpace.minPoint().y;
				y <= nmsSpace.maxPoint().y; y += gridSizes.y)
		{
			const float yf = y * scaleToWorld;
			for (glm::vec4::value_type z = nmsSpace.minPoint().z;
					z <= nmsSpace.maxPoint().z; z += gridSizes.z)
			{
				const float zf = z * scaleToWorld;

				mGrid.push_back({ xf, nmsSpace.minPoint().y * scaleToWorld, zf });
				mGrid.push_back({ xf, nmsSpace.maxPoint().y * scaleToWorld, zf });

				// horozontal lines
				mGrid.push_back({ nmsSpace.minPoint().x * scaleToWorld, yf, zf });
				mGrid.push_back({ nmsSpace.maxPoint().x * scaleToWorld, yf, zf });

				mGrid.push_back({ xf, yf, nmsSpace.minPoint().z * scaleToWorld });
				mGrid.push_back({ xf, yf, nmsSpace.maxPoint().z * scaleToWorld });
			}
		}
	}
}

float normalise(float value, float max)
{
	return float(2.0 * value / max - 1.0);
}

void NoMansSky::loadStars(const std::string& fileName,
						  const AABB& OW_UNUSED(nmsSpace),
						  float scaleToWorld)
{
	int fontHeight = 12;
	glm::vec2 nice = { 0.00625f, 2 * 0.00625f };

	std::string line;
	std::ifstream myfile(fileName);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line.size() == 0) // Test for empty line (CR?)
				break;
			if (line[0] == '#')
				continue;
			glm::vec4 point(0);
			std::vector<std::string> elms = OWUtils::split(line, ',');

			if (elms.size() == 2)
			{
				std::vector<std::string> coords = OWUtils::split(elms[1], ':');
				for (auto& s : coords)
					OWUtils::trim(s);
				if (coords.size() == 5)
				{
					// Signal Booster: AAAA:XXXX:YYYY:ZZZZ:SSSS
					// AAAA - something to do with the location on a planet's surface
					// SSSS - is the index of the system within the galactic region. Each region is, roughly, a 400 LY cube.
					//      - Same as Portal address System ID
					coords.erase(coords.begin());
				}

				if (coords.size() == 4)
				{
					// https://nomanssky.gamepedia.com/Portal_address#Galactic_Coordinates_System
					// Values begin at the bottom northwest corner of the chart and end at the 
					// top southeast corner, at Delta Majoris, where it ends 
					// at 0FFE:00FE : 0FFE : 0000
					// format is label, galactic coords: screenX:screenY:z:w
					// This mapping to the Game world asuming 0,0,0 is at the center.
					__int64 x = std::stoull(coords[0], 0, 16) - 0x7FF;
					__int64 y = std::stoull(coords[1], 0, 16) - 0x7F;
					__int64 z = std::stoull(coords[2], 0, 16) - 0x7FF;
					point.x = static_cast<glm::vec4::value_type>(x);
					point.y = static_cast<glm::vec4::value_type>(y);
					point.z = static_cast<glm::vec4::value_type>(z);
				}
				else if (coords.size() == 1)
				{
					// https://nomanssky.gamepedia.com/Portal_address#Portal_Coordinates_System
					std::string portalNum = coords[0];
					if (portalNum.size() == 12)
					{
						// Portal Coordinates can be confirmed at 
						// https://pahefu.github.io/pilgrimstarpath/
												
						// format is label, PortalAddress. Comes from the 12 Glyph 
						// characters that a Portal Terminal uses. Format is:
						// Planet Index(1)/Solar System Index(3)/Voxel Position(8)
						// Voxel Position x=0, y=0, z=0 is at the center.
						//				Y Values. Y axis is height. Ranges from [01, FF)
						// Increases from 0 to Up Pole at 7F
						// Starts again at Down Pole at 81 increasing to FF (which is back at the center)
						// Entering Y=80 returns Y=81.
						//				X Values. [001, FFF]
						// X Values have East and West Poles. They increase easterly beginning 
						// with 0 at the center. 7FF is  the East (right) Pole
						// Resumes with West Pole = X=801 and increasing towards the center.
						// Entering X=800 returns X=801
						//				Z Values. [001, FFF] (details same as X)
						// Z values have South and North Poles. They increase southerly until
						// z=7FF then numbering resumes from the northern pole with Z=801 and 
						// increasing as it nears centre

						// PSSSYYZZZXXX
							
						// P -  Planet Index in a system/ Max value == 6
						std::string planetIndex = portalNum.substr(0, 1);
						// SSS - Solar System Index. The address for a specific 
						// star system within the given region. SO far the max 
						// number of systems in a region iss 555
						std::string solarSystemIndex = portalNum.substr(1, 3);

						// x,y,z voxel positions
						std::string sY = portalNum.substr(4, 2);
						std::string sZ = portalNum.substr(6, 3);
						std::string sX = portalNum.substr(9, 3);

						// This mapping to the Game world asuming 0,0,0 is at the center.
						__int64 x = std::stoull(sX, 0, 16);
						if (x > 0x7FF)
							x -= 0xFFF;

						__int64 y = std::stoull(sY, 0, 16);
						if (y > 0x7F)
							y -= 0xFF;

						__int64 z = std::stoull(sZ, 0, 16);
						if (z > 0x7FF)
							z -= 0xFFF;
						point.x = static_cast<glm::vec4::value_type>(x);
						point.y = static_cast<glm::vec4::value_type>(y);
						point.z = static_cast<glm::vec4::value_type>(z);

					}
				}
			}
			else if (elms.size() == 4)
			{
				// format is label, screenX, screenY, z
				point.x = static_cast<glm::vec4::value_type>(std::stoull(elms[1]));
				point.y = 0;
				point.z = static_cast<glm::vec4::value_type>(std::stoull(elms[2]));
			}
			point.x *= scaleToWorld;
			point.y *= scaleToWorld;
			point.z *= scaleToWorld;
			point.w = 1.0;
			//TextBillboard* text = new TextBillboardFixed( { point.x, point.y, point.z }, );
			TextData td;
			td.text(elms[0]);
			td.font("arial.ttf", fontHeight);
			td.colour({ 0.0, 0.0, 0.0, 1.0f });
			td.spacing(nice.x, nice.y, { 1.0,1.0 }, TextData::Right);
			TextRendererStatic* r = new TextRendererStatic();
			r->setup(&td, glm::vec3(point.x, point.y, point.z));
			mStarLabels.push_back(r);
		}

		myfile.close();
	}
	else
	{
		throw NMSException(std::stringstream() << "File [" << fileName << "] not loaded.");
	}
}

std::vector<glm::vec3> NoMansSky::createRandomVectors(const AABB& nmsSpace,
					unsigned int count, float scaleToWorld)
{
	std::vector<glm::vec3> retval;
	std::default_random_engine generator;
	// 3 times std dev either side of mean.
	float xstddev = nmsSpace.size().x * scaleToWorld / 6.0f;
	float ystddev = nmsSpace.size().y * scaleToWorld / 6.0f;
	float zstddev = nmsSpace.size().z * scaleToWorld / 6.0f;

	std::normal_distribution<float> xdistribution(0.0f, xstddev);
	std::normal_distribution<float> ydistribution(0.0f, ystddev);
	std::normal_distribution<float> zdistribution(0.0f, zstddev);

	std::vector<float> x(count);
	std::vector<float> y(count);
	std::vector<float> z(count);
	for (unsigned int i = 0; i < count; i++)
	{
		x[i] = xdistribution(generator);
		y[i] = ydistribution(generator);
		z[i] = zdistribution(generator);
	}

	for (unsigned int i = 0; i < count; i++)
	{
		retval.push_back({ x[i], y[i], z[i] });
	}
	return retval;
}

void NoMansSky::render(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& model)
{
	glm::mat4 pvm = proj * view * model;
#ifdef DEBUG_GRID
	mGridShader->use();
	mGridShader->setMatrix4("pvm", pvm);
	glBindVertexArray(mVao[0]);
	mGridShader->setVector4f("uColour", { 0, 1.0, 0.5, 1 });
	glDrawArrays(GL_LINES, 0, GLsizei(mGrid.size()));
	glBindVertexArray(0);
#endif

#ifdef DEBUG_STARS
	auto pointRender = [](const glm::mat4& OW_UNUSED(proj), const glm::mat4& view,
		const glm::mat4& OW_UNUSED(model), const Shader* shader) {
		glm::vec3 CameraRight_worldspace =
		{ view[0][0], view[1][0], view[2][0] };
		shader->use();
		shader->setVector3f("CameraRight_worldspace", CameraRight_worldspace);
		glm::vec3 CameraUp_worldspace = { view[0][1], view[1][1], view[2][1] };
		shader->setVector3f("CameraUp_worldspace", CameraUp_worldspace);
		shader->setFloat("u_time", globals->secondsSinceLoad());
		glm::vec2 v2 = globals->pointingDevicePosition();
		shader->setVector2f("u_mouse", v2);
	};
	mStarRenderer->render(proj, view, model, nullptr, pointRender);
	for (int i = 0; i < mStarLabels.size(); i++)
	{
		const TextRenderer* sr = mStarLabels[i];
		sr->render(proj, view, model);
	}
#endif
}

void NoMansSky::readSaveFile(const std::string& saveFileMeta, const std::string& saveFile)
{

}
