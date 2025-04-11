#include "NoMansSkyStarMap.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <regex>
#include <random>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <json/single_include/nlohmann/json.hpp>


#include <Core/CommonUtils.h>
#include <Core/ErrorHandling.h>
#include <Core/GlobalSettings.h>

#include <Geometry/GeometricShapes.h>
#include <Component/TextComponent.h>

#include <Renderers/MeshRenderer.h>

#define DEBUG_GRID
#define DEBUG_STAR_LABELS
#define DEBUG_STARS

NoMansSky::NoMansSky(Scene* _scene, const std::string& _name)
: OWActorDiscrete(_scene, _name, nullptr)
{
}

void NoMansSky::initialise(const NoMansSkyData& _data)
{
	mStarRadius = glm::vec2(0, 0);
	mData = _data;

	const std::string& fileName = mData.starFile;
	const AABB& world = mData.starWorld;
	this->transform(new OWTransform());
	glm::u32vec3 gridSizes({ 0xAA, 0xAA, 0xAA });
	AABB NMSSize(glm::vec3(-0x7FF, -0x7F, -0x7FF),
		glm::vec3(0x7FF, 0x7F, 0x7FF));
	float scaleNMStoWorld = world.size().x / NMSSize.size().x;
#ifdef DEBUG_GRID
	{
		mData.gridShader.shaderV = "Lines.v.glsl";
		mData.gridShader.shaderF = "Lines.f.glsl";
		mData.gridShader.PVMName = "pv";
		mData.gridShader.uniforms.push_back({ ShaderDataUniforms::UV4F, "uColour",
					OWUtils::to_string(glm::vec4(0.90f, 0.91f, 0.98f, 1.0f)) }); // silver
		Shader * shader = new Shader(mData.gridShader);
		OWActorDiscrete::DiscreteEntity sse;
		sse.colour = glm::vec4(0.90f, 0.91f, 0.98f, 1.0f); // silver;
		sse.coll = new OWCollider(this, OWCollider::CollisionType::Permeable);
		sse.mesh = (new OWMeshComponent(this, "Grid"))
			->add(MeshData()
			.addVertices(createGrid(NMSSize, gridSizes, scaleNMStoWorld))
			.setModes(GL_LINES, GL_LINES, GL_FILL));
		sse.rend = new OWMeshRenderer(shader, { GPUBufferObject::BufferType::Model, GPUBufferObject::BufferType::Colour },
			GPUBufferObject::BufferStyle::SSBO);
		sse.physics = new OWPhysics(new OWTransform());
		addComponents(sse);
	}
#endif
	loadStars(fileName, NMSSize, scaleNMStoWorld);
#ifdef DEBUG_STARS
	mStarRadius = mData.starRadius;
	std::vector<glm::vec3> squareVertices =
		OWGeometricShapes::star(mStarRadius.x / 5.0f, mStarRadius.x / 3.3f, 15);
	//		GeometricShapes::rectangle(mStarRadius * 2.0f, -mStarRadius);
	std::vector<glm::vec4> instanceColours;
	while (instanceColours.size() < mData.numberOfStars)
	{
		instanceColours.push_back(OWUtils::randomSolidColour());
	}
	Shader* starShader = new Shader("instanced.v.glsl", "glow.f.glsl", "");
	starShader->setStandardUniformNames("pv");
	starShader->setUniform(ShaderDataUniforms::UniformType::UFloat,
		"cutoffRadius", OWUtils::to_string(mStarRadius.x), true);
	glm::vec2 w = globals->physicalWindowSize();
	auto pointRender = [w](
		const glm::mat4& OW_UNUSED(proj),
		const glm::mat4& view,
		const glm::vec3& OW_UNUSED(cameraPos),
		const Shader* shader)
		{
			glm::vec3 CameraRight_worldspace = { view[0][0], view[1][0], view[2][0] };
			shader->use();
			shader->setVector3f("CameraRight_worldspace", CameraRight_worldspace);
			glm::vec3 CameraUp_worldspace = { view[0][1], view[1][1], view[2][1] };
			shader->setVector3f("CameraUp_worldspace", CameraUp_worldspace);
			shader->setFloat("u_time", globals->secondsSinceLoad());
			shader->setVector2f("u_resolution", w);
		};
	starShader->appendMutator(pointRender);
	OWActorDiscrete::DiscreteEntity sse1;
	sse1.coll = new OWCollider(this, OWCollider::CollisionType::Permeable);
	sse1.mesh = (new OWMeshComponent(this, "Star Template"))
		->add(MeshData()
		.addVertices(squareVertices)
		.setModes(GL_TRIANGLES));

	sse1.rend = (new OWMeshRenderer(starShader, 
			{ GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour },
			GPUBufferObject::BufferStyle::SSBO))
		->addToSSBO(createRandomVectors(NMSSize, mData.numberOfStars, scaleNMStoWorld), GPUBufferObject::BufferType::Position)
		->addToSSBO(instanceColours, GPUBufferObject::BufferType::Colour)
		->lockSSBO({ GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour });
	sse1.physics = new OWPhysics(new OWTransform());
	addComponents(sse1);
#endif
}

std::vector<glm::vec3> NoMansSky::createGrid(const AABB& nmsSpace,
						   const glm::u32vec3& gridSizes,
						   float scaleToWorld)
{
	// https://nomanssky.gamepedia.com/Portal_address
	std::vector<glm::vec3> grid;
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

				grid.push_back({ xf, nmsSpace.minPoint().y * scaleToWorld, zf });
				grid.push_back({ xf, nmsSpace.maxPoint().y * scaleToWorld, zf });

				// horozontal lines
				grid.push_back({ nmsSpace.minPoint().x * scaleToWorld, yf, zf });
				grid.push_back({ nmsSpace.maxPoint().x * scaleToWorld, yf, zf });

				grid.push_back({ xf, yf, nmsSpace.minPoint().z * scaleToWorld });
				grid.push_back({ xf, yf, nmsSpace.maxPoint().z * scaleToWorld });
			}
		}
	}
	return grid;
}

void NoMansSky::loadStars(const std::string& fileName,
						  const AABB& OW_UNUSED(nmsSpace),
						  float scaleToWorld)
{
	const glm::vec2 niceFontSpacing = { 0.00625f, 2 * 0.00625f };
#ifdef DEBUG_STAR_LABELS
	OWActorNCom1Ren* starLabels = new OWActorNCom1Ren(this->scene(), "Star Labels", this);
	starLabels->transform(new OWTransform());
	Shader* shader = new Shader("textStaticBillboard.v.glsl", "text.f.glsl", "");
	shader->setStandardUniformNames("pv");
	shader->appendMutator(OWTextComponent::shaderMutator(OWRenderTypes::DrawType::TwoDStatic));

	starLabels->renderer(new OWMeshRenderer(shader, 
		{ GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour,
				GPUBufferObject::BufferType::BillboardSize },
		GPUBufferObject::BufferStyle::SSBO));
#endif
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
#ifdef DEBUG_STAR_LABELS
			OWTextComponentData td;
			td.tdt = OWRenderTypes::DrawType::TwoDStatic;
			td.text = elms[0];
			td.colour = { 0.0, 0.0, 0.0, 1.0f };
			td.fontSpacing = niceFontSpacing;
			td.referencePos = OWTextComponentData::PositionType::Right;
			OWActorNCom1Ren::NCom1RenElement elm;
			elm.mesh = new OWTextComponent(starLabels, td.text, td);
			elm.physics = new OWPhysics(new OWTransform({ point, glm::vec3(0.5f, 0.5f, 0.5f) }));
			elm.coll = new OWCollider(starLabels, OWCollider::CollisionType::Permeable);
			elm.colour = OWUtils::randomSolidColour();
			starLabels->addComponents(elm);
#endif
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

