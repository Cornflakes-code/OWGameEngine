#pragma once

#include <Core/BoundingBox.h>

#include <Helpers/ModelData.h>

namespace NMS
{
	ModelData createAxisData(const AABB& world);
	std::pair<glm::vec3, glm::vec3> boundingBox(const std::vector<glm::vec3>& points);
	glm::vec3 center(const std::vector<glm::vec3>& points);
	glm::vec3 center(const glm::vec3& p1, const glm::vec3& p2);
	ModelData createRopeEnds(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	ModelData createRopeLines(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	ModelData createRopeSurfaces(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	RendererBase* createLightSource(const glm::vec3& position = glm::vec3(0,0,0));
}