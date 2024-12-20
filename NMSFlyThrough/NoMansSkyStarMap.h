#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <Geometry/BoundingBox.h>

#include <Core/OWActor.h>

class Shader;
class TextRenderer;
/*
	The NMS game is a simple fly through of Solar Systems visited while
	playing the No Man's Sky game. Will be moved to a different repo in due course.
*/

class NoMansSky: public OWActor
{
	glm::vec2 mStarRadius;
	std::vector<glm::vec3> mRandomMinorStars;
	std::vector<glm::vec4> mStarPositions;
	std::vector<glm::vec4> mStarColours;
	std::vector<glm::vec3> mGrid;

	void loadStars(const std::string& fileName, 
				   const AABB& nmsSpace,
				   float scaleToWorld);
	void createGrid(const AABB& nmsSpace,
					const glm::u32vec3& gridSizes,
					float scaleToWorld);
	std::vector<glm::vec3> createRandomVectors(const AABB& world,
					unsigned int count, float scaleToWorld);
public:
	NoMansSky(Scene* _owner, const glm::vec3& _position);
	void setUp(const std::string& fileName, const AABB& world);
	void readSaveFile(const std::string& saveFileMeta, 
			const std::string& saveFile);
};