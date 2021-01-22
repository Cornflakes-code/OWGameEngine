#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <Core/BoundingBox.h>

#include <Renderers/LightRenderer.h>

class Shader;
class InstanceRenderer;
class TextRenderer;
/*
	The NMS game is a simple fly through of Solar Systems visited while
	playing the No Man's Sky game. Will be moved to a different repo in due course.
*/
class NoMansSky
{
	InstanceRenderer* mStarRenderer;
	LightRenderer mGridRenderer;
	std::vector<glm::vec3> mRandomMinorStars;
	std::vector<glm::vec4> mStarPositions;
	std::vector<TextRenderer*> mStarLabels;
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
	Shader* mStarShader;

	GLuint mVao[2];
	GLuint mVboGridPos;
	GLuint mVboGridCol;
public:
	NoMansSky();
	void setUp(const std::string& fileName, const AABB& world);
	void render(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& model);
	void readSaveFile(const std::string& saveFileMeta, const std::string& saveFile);
};