#include "NMSUtils.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <Core/Camera.h>
#include <Core/GlobalSettings.h>

#include <Helpers/FreeTypeFontAtlas.h>
#include <Helpers/MeshDataLight.h>
#include <Helpers/Shader.h>
#include <Helpers/ShaderFactory.h>
#include <Helpers/TextData.h>
#include <Helpers/GeometricShapes.h>
#include <Renderers/LightRenderer.h>
#include <Renderers/VAOBuffer.h>
#include <Renderers/TextRendererStatic.h>

#include "RopeNormaliser.h"

namespace NMS
{
	ModelData createAxisData(const AABB& w)
	{
		int fontHeight = 12;
		glm::vec2 nice = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
			fontHeight, globals->camera()->aspectRatio());

		float scale = 1.0;

		TextData td;
		td.font("arial.ttf", fontHeight);
		td.colour({ 0.3, 0.45, 0.7, 1.0f });
		td.spacing(nice.x, nice.y, glm::vec2(scale, scale),
			TextData::Top | TextData::Right);
		td.text("0");

		std::vector<glm::vec3> axisCoords = {
			{ w.center().x, w.center().y, w.center().z },
			{ w.maxPoint().x * scale, 0.0, 0.0 },
			{ 0.0, w.maxPoint().y * scale, 0.0 },
			{ 0.0, 0.0, w.maxPoint().z * scale} };

		TextRenderer* zero = new TextRendererStatic();
		zero->setup(&td, glm::vec3(0));
		ModelData md;
		md.renderers.push_back(zero);
		{
			TextRenderer* x = new TextRendererStatic();
			td.text("X");
			td.spacing(nice.x, nice.y, glm::vec2(scale, scale), TextData::Center);
			td.colour({ 1.0, 0.0, 0.0, 1.0f });
			x->setup(&td, axisCoords[1]);
			md.renderers.push_back(x);
		}
		{
			TextRenderer* y = new TextRendererStatic();
			td.text("Y");
			td.spacing(nice.x, nice.y, glm::vec2(scale, scale), TextData::Center);
			y->setup(&td, axisCoords[2]);
			md.renderers.push_back(y);
		}
		{
			TextRenderer* z = new TextRendererStatic();
			td.text("Z");
			td.spacing(nice.x, nice.y, glm::vec2(scale, scale), TextData::Center);
			z->setup(&td, axisCoords[3]);
			md.renderers.push_back(z);
		}
		ShaderFactory shaders;
		Shader* lineShader = new Shader();
		lineShader->loadShaders(shaders.boilerPlateVertexShader(),
			shaders.boilerPlateFragmentShader(),
			shaders.boilerPlateGeometryShader());
		lineShader->setStandardUniformNames("pvm");
		MeshDataLight lineData;
		lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_GREEN), "colour");
		lineData.vertices(axisCoords, GL_LINES);
		lineData.indices({ 0,1, 0,2, 0,3 }, GL_LINES);
		LightRenderer* lines = new LightRenderer(lineShader);
		lines->setup(&lineData);
		md.renderers.push_back(lines);
		return md;
	}
	glm::vec3 center(const glm::vec3& p1, const glm::vec3& p2)
	{
		return glm::vec3((p1.x + p2.x) / 2.0f,
			(p1.y + p2.y) / 2.0f,
			(p1.z + p2.z) / 2.0f);
	}

	glm::vec3 center(const std::vector<glm::vec3>& points)
	{
		std::pair<glm::vec3, glm::vec3> bb = boundingBox(points);
		return center(bb.first, bb.second);
	}

	std::pair<glm::vec3, glm::vec3> boundingBox(const std::vector<glm::vec3>& points)
	{
		std::pair<glm::vec3, glm::vec3> retval =
		{
			glm::vec3(std::numeric_limits<float>::max(),
					std::numeric_limits<float>::max(),
					std::numeric_limits<float>::max()),
			glm::vec3(-std::numeric_limits<float>::max(),
					-std::numeric_limits<float>::max(),
					-std::numeric_limits<float>::max())
		};
		for (const glm::vec3& pt : points)
		{
			retval.first.x = std::min(pt.x, retval.first.x);
			retval.first.y = std::min(pt.y, retval.first.y);
			retval.first.z = std::min(pt.z, retval.first.z);

			retval.second.x = std::max(pt.x, retval.second.x);
			retval.second.y = std::max(pt.y, retval.second.y);
			retval.second.z = std::max(pt.z, retval.second.z);
		}
		return retval;
	}

	ModelData createRopeEnds(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires)
	{
		ShaderFactory shaders;
		Shader* lineShader = new Shader();
		lineShader->loadShaders(shaders.boilerPlateVertexShader(),
			shaders.boilerPlateFragmentShader(),
			shaders.boilerPlateGeometryShader());
		lineShader->setStandardUniformNames("pvm");

		// Prepare the wire cross sections
		ModelData md;
		std::vector<glm::vec3> coords;
		for (auto& slice : threeDWires)
		{
			for (auto& polygon : slice)
			{
				MeshDataLight lineData;
				lineData.colour(OWUtils::colour(OWUtils::SolidColours::RED), "colour");
				lineData.vertices(polygon, GL_LINE_LOOP);
				LightRenderer* lines = new LightRenderer(lineShader);
				lines->setup(&lineData);
				md.renderers.push_back(lines);
			}
			break;
		}
		return md;
	}

	ModelData createRopeLines(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires)
	{
		ShaderFactory shaders;
		Shader* lineShader = new Shader();
		lineShader->loadShaders(shaders.boilerPlateVertexShader(),
			shaders.boilerPlateFragmentShader(),
			shaders.boilerPlateGeometryShader());
		lineShader->setStandardUniformNames("pvm");

		ModelData md;

		bool drawCenters = false;
		bool drawLines = true;

		size_t numLayers = threeDWires.size();
		size_t numWiresInEachLayer = threeDWires[0].size();
		size_t numPointsPerSlice = threeDWires[0][0].size() + 10;
		std::vector< std::vector<glm::vec3>> ropeLines;
		std::vector< std::vector<glm::vec3>> surfaces;
		for (size_t eachWire = 0; eachWire < numWiresInEachLayer; eachWire++)
		{
			if (drawCenters)
			{
				std::vector<glm::vec3> line;
				for (int layer = 0; layer < numLayers; layer++)
				{
					std::vector<glm::vec3>& slice = threeDWires[layer][eachWire];
					line.push_back(NMS::center(slice));
				}
				ropeLines.push_back(line);
			}
			if (drawLines)
			{
				for (int pointOnPoly = 0; pointOnPoly < numPointsPerSlice; pointOnPoly++)
				{
					std::vector<glm::vec3> line;
					for (int layer = 0; layer < numLayers; layer++)
					{
						if (pointOnPoly < threeDWires[layer][eachWire].size())
						{
							glm::vec3 pt = threeDWires[layer][eachWire][pointOnPoly];
							line.push_back(pt);
						}
					}
					if (line.size())
						ropeLines.push_back(line);
				}
			}
		}
		if (drawLines)
		{
			for (std::vector<glm::vec3>& aLine : ropeLines)
			{
				MeshDataLight lineData;
				lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED), "colour");
				lineData.vertices(aLine, GL_LINE_STRIP);
				//		lineData.indices({ 0,1, 0,2, 0,3 }, GL_LINES);
				LightRenderer* lines = new LightRenderer(lineShader);
				lines->setup(&lineData);
				md.renderers.push_back(lines);
			}
		}
		return md;
	}

	glm::vec3 centerOfPolygon(const std::vector<glm::vec3>& polygon)
	{
		glm::vec3 firstPolyCoord = polygon[0];
		glm::vec3 oppositePolyCoord = polygon[polygon.size() / 2];
		glm::vec3 midpoint = (firstPolyCoord + oppositePolyCoord) * 0.5f;
		return midpoint;
	}

	static unsigned int append(std::vector<glm::vec3>& v, const glm::vec3& p)
	{
		v.emplace_back(p);
		v.emplace_back(glm::vec3(0.0f));// placeholder for normal values
		return static_cast<unsigned int>(v.size()) - 2;
	}

	static unsigned int safeWrap(const std::vector<unsigned int>& vv, size_t ndx)
	{
		int i = static_cast<int>(ndx) - static_cast<int>(vv.size());
		return i < 0 ? vv[ndx] : vv[i];
	}

	ModelData createRopeSurfaces(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires)
	{
		Shader* wireShader = new Shader();
		wireShader->loadShaders("Wires.v.glsl",
			"Wires.f.glsl",
			ShaderFactory::boilerPlateGeometryShader());
		wireShader->setStandardUniformNames("", "projection", "view", "model");

		// Prepare the wire cross sections
		ModelData md;
		size_t numLayers = threeDWires.size();

		// The number of wires for each layer is the same
		size_t numWires = threeDWires[0].size();
		// This holds the points that go to the gpu
		std::vector<glm::vec3> triAnglePoints;

		/*
		* For each wire mirror the structure of threeDWires, but populate
		* with the indices of the corresponding point pushed back into triAnglePoints.
		*/
		std::vector<std::vector<std::vector<unsigned int>>> tempWireIndices(threeDWires.size());
		for (int i = 0; i < threeDWires.size(); i++)
		{
			tempWireIndices[i] = std::vector<std::vector<unsigned int>>(threeDWires[i].size());
			for (int j = 0; j < threeDWires[i].size(); j++)
			{
				tempWireIndices[i][j] = std::vector<unsigned int>(threeDWires[i][j].size());
				for (unsigned int pointOnPoly = 0; pointOnPoly < threeDWires[i][j].size(); pointOnPoly++)
				{
					unsigned int ndx = append(triAnglePoints, threeDWires[i][j][pointOnPoly]);
					tempWireIndices[i][j][pointOnPoly] = ndx;
				}
			}
		}

		// Remember the centroid of each wire end so we can draw triangles on the ends.
		std::vector<std::vector<unsigned int>> centroidIndex(numWires, std::vector<unsigned int>(2, 0));
		// Populate the indice vectors
		for (int eachWire = 0; eachWire < threeDWires[0].size(); eachWire++)
		{
			/*
			* Find the rough centroids of the first and last polygon slices
				(layer == 0 and layer == numLayers - 1) respectively. We use these to
			* triangulate the surfaces of the end points.
			*/
			centroidIndex[eachWire][0] = append(triAnglePoints,
				centerOfPolygon(threeDWires[0][eachWire]));

			// Center of the bottom slice
			centroidIndex[eachWire][1] = append(triAnglePoints,
				centerOfPolygon(threeDWires[numLayers - 1][eachWire]));
		}

		RopeNormaliser rn(&triAnglePoints);
		// Now build the triangles
		for (int eachWire = 0; eachWire < tempWireIndices[0].size(); eachWire++)
		{
			for (int layer = 0; layer < numLayers; layer++)
			{
				size_t polySize = tempWireIndices[layer][eachWire].size();
				for (size_t pointOnPoly = 0; pointOnPoly < polySize; pointOnPoly++)
				{
					if (layer < (numLayers - 1))
					{
						if (layer == 0)
						{
							// Form a triangle on the flat surface of the endpoint
							rn.appendTriangle(
								centroidIndex[eachWire][0],
								safeWrap(tempWireIndices[layer][eachWire], pointOnPoly),
								safeWrap(tempWireIndices[layer][eachWire], pointOnPoly + 1));
						}
						// Now for the triangles down the side of the wire
						rn.appendTriangle(
							safeWrap(tempWireIndices[layer][eachWire], pointOnPoly + 1),
							safeWrap(tempWireIndices[layer][eachWire], pointOnPoly),
							safeWrap(tempWireIndices[layer + 1][eachWire], pointOnPoly));

						rn.appendTriangle(
							safeWrap(tempWireIndices[layer][eachWire], pointOnPoly + 1),
							safeWrap(tempWireIndices[layer + 1][eachWire], pointOnPoly),
							safeWrap(tempWireIndices[layer + 1][eachWire], pointOnPoly + 1));

						// The next layers polygon may have more points than this layer.
						// If we are the lasy point in this layer and the next layer has 
						// more points then fill the gaps.
						if ((pointOnPoly + 1 == polySize) &&
							(pointOnPoly + 1 < tempWireIndices[layer + 1][eachWire].size()))
						{
							size_t a = pointOnPoly + 1;
							while (a < tempWireIndices[layer + 1][eachWire].size())
							{
								rn.appendTriangle(
									safeWrap(tempWireIndices[layer + 1][eachWire], a + 1),
									safeWrap(tempWireIndices[layer][eachWire], pointOnPoly + 1),
									safeWrap(tempWireIndices[layer + 1][eachWire], a));
								a++;
							}
						}
					}
					else if (layer == (numLayers - 1))
					{
						// Now form triangles on the flat surface of the end of the wire.
						rn.appendTriangle(
							safeWrap(
								tempWireIndices[layer][eachWire], pointOnPoly + 1),
							tempWireIndices[layer][eachWire][pointOnPoly],
							centroidIndex[eachWire][1]);

					}
				}
				//	break;
			}
		}

		std::vector<std::pair<unsigned int, glm::vec3>> normalPairs = rn.createNormals();
		for (const auto& each : normalPairs)
		{
			unsigned int ndx = each.first;
			glm::vec3 normal = each.second;
			triAnglePoints[ndx + 1] = normal;
		}
		VAOBuffer* vao = new VAOBuffer(wireShader);
		MeshDataLight lineData;
		lineData.vertices(triAnglePoints, GL_TRIANGLES);
		lineData.indices(RopeNormaliser::mIndexBuffer, GL_TRIANGLES);
		lineData.polygonMode(GL_FILL);
		//	lineData.polygonMode(GL_LINE);
		vao->add(&lineData);
		vao->prepare();
		md.renderers.push_back(vao);
		return md;
	}

	RendererBase* createLightSource(const glm::vec3& position)
	{
		/*
		* An alternate way to draw spheres with triangles is at:
		* http://www.songho.ca/opengl/gl_sphere.html
		*/
		Shader* sh = new Shader();
		sh->loadShaders(
			"sphere.v.glsl",
			"sphere.f.glsl",
			ShaderFactory::boilerPlateGeometryShader());
		sh->setStandardUniformNames("pvm", "perspective", "view", "model", "camPos");

		sh->setVector3f("sphereCenter", position, true);
		MeshDataLight lineData;
		std::vector<glm::vec3> vertices = GeometricShapes::cube(position);
		lineData.vertices(vertices, GL_TRIANGLES);

		VAOBuffer* vao = new VAOBuffer(sh);
		vao->add(&lineData);
		vao->prepare();
		return vao;
	}
}

