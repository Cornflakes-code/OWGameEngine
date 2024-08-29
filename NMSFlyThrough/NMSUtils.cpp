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

#include <Renderers/LightRenderer.h>
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

		MeshDataLight lineData;
		lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_GREEN), "colour");
		lineData.vertices(axisCoords, GL_LINES);
		lineData.indices({ 0,1, 0,2, 0,3 }, GL_LINES);
		LightRenderer* lines = new LightRenderer(lineShader, "pvm");
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

	ModelData createRopeLines(std::vector<std::vector<std::vector<glm::vec3>>>& slices)
	{
		int fontHeight = 12;
		glm::vec2 nice = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
			fontHeight, globals->camera()->aspectRatio());

		float scale = 1.0;

		ShaderFactory shaders;
		Shader* lineShader = new Shader();
		lineShader->loadShaders(shaders.boilerPlateVertexShader(),
			shaders.boilerPlateFragmentShader(),
			shaders.boilerPlateGeometryShader());

		// Prepare the wire cross sections
		ModelData md;
		std::vector<glm::vec3> coords;
		for (auto& wire : slices)
		{
			for (auto& polygon : wire)
			{
				MeshDataLight lineData;
				lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_GREEN), "colour");
				lineData.vertices(polygon, GL_LINE_LOOP);
				LightRenderer* lines = new LightRenderer(lineShader, "pvm");
				lines->setup(&lineData);
				md.renderers.push_back(lines);
			}
		}
		for (std::vector<std::vector<glm::vec3>>& wire : slices)
		{
			if (wire.size() < 2)
				continue;
			std::vector<glm::vec3> line;
			
			for (std::vector<glm::vec3>& slice : wire)
			{
				line.push_back(NMS::center(slice));
			}
			MeshDataLight lineData;
			lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED), "colour");
			lineData.vertices(line, GL_LINE_STRIP);
			//		lineData.indices({ 0,1, 0,2, 0,3 }, GL_LINES);
			LightRenderer* lines = new LightRenderer(lineShader, "pvm");
			lines->setup(&lineData);
			md.renderers.push_back(lines);
		}
		return md;
	}
}

