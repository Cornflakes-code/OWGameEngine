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

		int numLayers = threeDWires.size();
		int numWiresInEachLayer = threeDWires[0].size();
		int numPointsPerSlice = threeDWires[0][0].size() + 10;
		std::vector< std::vector<glm::vec3>> lines;
		std::vector< std::vector<glm::vec3>> surfaces;
		const size_t numWires = threeDWires[0].size();
		for (int eachWire = 0; eachWire < numWiresInEachLayer; eachWire++)
		{
			if (drawCenters)
			{
				std::vector<glm::vec3> line;
				for (int layer = 0; layer < numLayers; layer++)
				{
					std::vector<glm::vec3>& slice = threeDWires[layer][eachWire];
					line.push_back(NMS::center(slice));
				}
				lines.push_back(line);
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
						lines.push_back(line);
				}
			}
		}
		if (drawLines)
		{
			for (std::vector<glm::vec3>& aLine : lines)
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
		glm::vec3 oppositePolyCoord = polygon[(polygon.size() - 1) / 2];
		glm::vec3 midpoint = (firstPolyCoord + oppositePolyCoord) * 0.5f;
		return midpoint;
	}
	
	static unsigned int append(std::vector<glm::vec3>& v, const glm::vec3& p)
	{
		v.emplace_back(p);
		v.emplace_back(glm::vec3(0, 0, 0));// normal placeholder
		return v.size() - 2;
	}

	static unsigned int safeWrap(const std::vector<unsigned int>& vv, unsigned int ndx)
	{
		unsigned int p = ndx >= vv.size() ? 0 : ndx;
		return vv[p];
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
		std::vector<unsigned int> indexBuffer;
		// The number of layers for each wire does not change
		int numLayers = threeDWires.size();
		int numWires = threeDWires[0].size();
		std::vector< std::vector<glm::vec3>> lines;
		std::vector<glm::vec3> triAnglePoints;
		glm::vec3 normal = { 0.0f, 0.0f, 0.0f };
		/*
		* For each wire mirror the structure of threeDWires, but populate
		* with the indices of the corresponding point pushed back into triAnglePoints.
		*/
		std::vector<std::vector<std::vector<unsigned int>>> tempWireIndices(threeDWires.size());

		// Remember the centroid of each wire end
		std::vector<std::vector<unsigned int>> centroidIndex(numWires, std::vector<unsigned int>(2, 0));
		for (int i = 0; i < threeDWires.size(); i++)
		{
			tempWireIndices[i] = std::vector<std::vector<unsigned int>>(threeDWires[i].size());
			for (int j = 0; j < threeDWires[i].size(); j++)
			{
				tempWireIndices[i][j] = std::vector<unsigned int> (threeDWires[i][j].size());
			}
		}

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
			for (int layer = 0; layer < numLayers; layer++)
			{
				for (int pointOnPoly = 0; pointOnPoly < threeDWires[layer][eachWire].size(); pointOnPoly++)
				{
					unsigned int ndx = append(triAnglePoints, threeDWires[layer][eachWire][pointOnPoly]);
					tempWireIndices[layer][eachWire][pointOnPoly] = ndx;
				}
			}
		}
		// Now build the triangles
		for (int eachWire = 0; eachWire < threeDWires[0].size(); eachWire++)
		{
			for (int layer = 0; layer < numLayers; layer++)
			{
				unsigned int polySize = threeDWires[layer][eachWire].size();
				for (int pointOnPoly = 0; pointOnPoly < polySize; pointOnPoly++)
				{
					if (layer < (numLayers - 1))
					{
						if (layer == 0)
						{
							// Form a triangle on the flat surface of the endpoint
							indexBuffer.push_back(tempWireIndices[layer][eachWire][pointOnPoly]);
							indexBuffer.push_back(centroidIndex[eachWire][0]);
							indexBuffer.push_back(safeWrap(tempWireIndices[layer][eachWire], pointOnPoly + 1));
						}
						// Now for the triangles down the side of the wire
						indexBuffer.push_back(tempWireIndices[layer][eachWire][pointOnPoly]);
						indexBuffer.push_back(safeWrap(tempWireIndices[layer][eachWire], pointOnPoly+1));
						indexBuffer.push_back(safeWrap(tempWireIndices[layer + 1][eachWire], pointOnPoly));

						indexBuffer.push_back(safeWrap(tempWireIndices[layer + 1][eachWire], pointOnPoly));
						indexBuffer.push_back(safeWrap(tempWireIndices[layer][eachWire], pointOnPoly + 1));
						indexBuffer.push_back(safeWrap(tempWireIndices[layer + 1][eachWire], pointOnPoly + 1));

						// The next layers polygon may have more points than this layer.
						if ((pointOnPoly + 1 == polySize) && (pointOnPoly + 1 < tempWireIndices[layer + 1][eachWire].size()))
						{
							unsigned int a = pointOnPoly + 1;
							while (a < tempWireIndices[layer + 1][eachWire].size())
							{
								indexBuffer.push_back(safeWrap(tempWireIndices[layer][eachWire], pointOnPoly+1));
								indexBuffer.push_back(safeWrap(tempWireIndices[layer + 1][eachWire], a + 1));
								indexBuffer.push_back(safeWrap(tempWireIndices[layer + 1][eachWire], a));
								a++;
							}
						}
					}
					else if (layer == (numLayers - 1))
					{
						// Only form a triangle on the flat surface of the endpoint
						indexBuffer.push_back(tempWireIndices[layer][eachWire][pointOnPoly]);
						indexBuffer.push_back(safeWrap(tempWireIndices[layer][eachWire], pointOnPoly + 1));
						indexBuffer.push_back(centroidIndex[eachWire][1]);
					}
				}
			}
		}

		VAOBuffer* vao = new VAOBuffer(wireShader);
		MeshDataLight lineData;
		//lineData.vertices(triAnglePoints, GL_TRIANGLE_STRIP);
		lineData.vertices(triAnglePoints, GL_TRIANGLES);
		lineData.indices(indexBuffer, GL_TRIANGLES);
		lineData.polygonMode(GL_LINE);
		vao->add(&lineData);
		vao->prepare();
		md.renderers.push_back(vao);
		return md;
	}
	
	static glm::vec3 safeWrap(const std::vector<glm::vec3>& vv, unsigned int ndx)
	{
		unsigned int p = ndx >= vv.size() ? 0 : ndx;
		return vv[p];
	}

	ModelData createRopeSurfacesOld(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires)
	{
		Shader* wireShader = new Shader();
		wireShader->loadShaders("Wires.v.glsl",
			"Wires.f.glsl",
			ShaderFactory::boilerPlateGeometryShader());
		wireShader->setStandardUniformNames("", "projection", "view", "model");

		// Prepare the wire cross sections
		ModelData md;
		std::vector<unsigned int> indexBuffer;
		// The number of layers for each wire does not change
		int numLayers = threeDWires.size();

		std::vector< std::vector<glm::vec3>> lines;
		std::vector<glm::vec3> triAnglePoints;
		glm::vec3 normal = { 0.0f, 0.0f, 0.0f };
		if (true)
		{
			for (int eachWire = 0; eachWire < threeDWires[0].size(); eachWire++)
			{
				/*
				* Find the rough centers of the first and last polygon slices 
				  (layer == 0 and layer == numLayers - 1) repsectively. We use these to 
				* triangulate the surfaces of the end points.
				*/
				glm::vec3 topMidpoint = centerOfPolygon(threeDWires[0][eachWire]);
				unsigned int topMidPointBufferIndex = append(triAnglePoints, topMidpoint);

				// Center of the bottom slice
				glm::vec3 bottomMidpoint = centerOfPolygon(threeDWires[numLayers - 1][eachWire]);
				unsigned int bottomMidPointBufferIndex = append(triAnglePoints, bottomMidpoint);
				for (int layer = 0; layer < numLayers; layer++)
				{
					for (int pointOnPoly = 0; pointOnPoly < threeDWires[layer][eachWire].size(); pointOnPoly++)
					{
						if (layer < (numLayers - 1))
						{
							if (layer == 0)
							{
								// Form a triangle on the flat surface of the endpoint
								unsigned int ndx00 = append(triAnglePoints, threeDWires[layer][eachWire][pointOnPoly]);
								indexBuffer.push_back(ndx00);
								indexBuffer.push_back(topMidPointBufferIndex);
								unsigned int ndx01 = append(triAnglePoints, safeWrap(threeDWires[layer][eachWire], pointOnPoly+1));
								indexBuffer.push_back(ndx01);

								// Now for the triangles down the side of the wire
								indexBuffer.push_back(ndx00);
								indexBuffer.push_back(ndx01);
								unsigned int ndx10 = append(triAnglePoints, safeWrap(threeDWires[layer+1][eachWire], pointOnPoly));
								indexBuffer.push_back(ndx10);

								indexBuffer.push_back(ndx10);
								indexBuffer.push_back(ndx01);
								unsigned int ndx11 = append(triAnglePoints, safeWrap(threeDWires[layer + 1][eachWire], pointOnPoly+1));
								indexBuffer.push_back(ndx11);
							}
							else if (false)
							{
								// Now for the triangles down the side of the wire
								unsigned int ndx00 = append(triAnglePoints, threeDWires[layer][eachWire][pointOnPoly]);
								indexBuffer.push_back(ndx00);
								unsigned int ndx01 = append(triAnglePoints, safeWrap(threeDWires[layer][eachWire], pointOnPoly + 1));
								indexBuffer.push_back(ndx01);
								unsigned int ndx10 = append(triAnglePoints, safeWrap(threeDWires[layer + 1][eachWire], pointOnPoly));
								indexBuffer.push_back(ndx10);

								indexBuffer.push_back(ndx10);
								indexBuffer.push_back(ndx01);
								unsigned int ndx11 = append(triAnglePoints, safeWrap(threeDWires[layer + 1][eachWire], pointOnPoly+1));
								indexBuffer.push_back(ndx11);
							}
						}
						else if (false && (layer == (numLayers - 1)))
						{
							// Only form a triangle on the flat surface of the endpoint
							unsigned int ndx00 = append(triAnglePoints, threeDWires[layer][eachWire][pointOnPoly]);
							indexBuffer.push_back(ndx00);
							unsigned int ndx01 = append(triAnglePoints, safeWrap(threeDWires[layer][eachWire], pointOnPoly+1));
							indexBuffer.push_back(ndx01);
							indexBuffer.push_back(bottomMidPointBufferIndex);
						}
					}
				}
			}
		}
		else
		{
			// This changes
			int numPointsPerSlice = threeDWires[0][0].size();
			for (int eachWire = 0; eachWire < threeDWires[0].size(); eachWire++)
			{
				for (int pointOnPoly = 0; pointOnPoly < numPointsPerSlice; pointOnPoly++)
				{
					for (int layer = 0; layer < (numLayers - 1); layer++)
					{
						// Setup for GL_TRIANGLESTRIP (now outdated)
						// https://stackoverflow.com/questions/20394727/gl-triangle-strip-vs-gl-triangle-fan
						int ct = triAnglePoints.size();
						int vCount = 2;
						int firstWire = pointOnPoly;
						if (firstWire >= threeDWires[layer][eachWire].size())
							firstWire = threeDWires[layer][eachWire].size() - 1;
						glm::vec3 pt = threeDWires[layer][eachWire][firstWire]; // A
						triAnglePoints.push_back(pt);
						if (vCount > 1)
							triAnglePoints.push_back(normal);

						int secondWire = firstWire + 1;
						if (secondWire >= threeDWires[layer][eachWire].size())
							secondWire = 0;
						pt = threeDWires[layer][eachWire][secondWire]; // B
						triAnglePoints.push_back(pt);
						if (vCount > 1)
							triAnglePoints.push_back(normal);

						if (firstWire >= threeDWires[layer + 1][eachWire].size())
							firstWire = threeDWires[layer + 1][eachWire].size() - 1;
						if (secondWire >= threeDWires[layer + 1][eachWire].size())
							secondWire = 0;
						pt = threeDWires[layer + 1][eachWire][firstWire]; // C
						triAnglePoints.push_back(pt);
						if (vCount > 1)
							triAnglePoints.push_back(normal);

						pt = threeDWires[layer + 1][eachWire][secondWire]; // D
						triAnglePoints.push_back(pt);
						if (vCount > 1)
							triAnglePoints.push_back(normal);

						indexBuffer.push_back(0 * vCount + ct); // ABC
						indexBuffer.push_back(1 * vCount + ct); // ABC
						indexBuffer.push_back(2 * vCount + ct); // ABC

						indexBuffer.push_back(1 * vCount + ct); // BDC
						indexBuffer.push_back(3 * vCount + ct); // BDC
						indexBuffer.push_back(2 * vCount + ct); // BDC
					}
				}
			}
		}

		VAOBuffer* vao = new VAOBuffer(wireShader);
		MeshDataLight lineData;
		//lineData.vertices(triAnglePoints, GL_TRIANGLE_STRIP);
		lineData.vertices(triAnglePoints, GL_TRIANGLES);
		lineData.indices(indexBuffer, GL_TRIANGLES);
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

