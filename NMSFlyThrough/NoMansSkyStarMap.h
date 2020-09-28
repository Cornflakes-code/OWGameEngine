#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <Renderables/BoundingBox.h>
#include <Renderables/VertexRenderer.h>

class Shader;

/*
	The NMS game is a simple fly through of Solar Systems visited while
	playing the No Man's Sky game. Will be moved to a different repo in due course.
*/
class NoMansSky
{
	VertexRenderer mStarRenderer;
	std::vector<glm::vec4> mStarPositions;
	std::vector<VertexRenderer> mStarLabels;
	std::vector<glm::vec4> mStarColours;
	std::vector<glm::vec3> mGrid;

	void loadStars(const std::string& fileName, 
				   const AABB& nmsSpace,
				   float scaleToWorld);
	void createGrid(const AABB& nmsSpace, 
					const glm::u32vec3& gridSizes,
					float scaleToWorld);
	void createRandomStars(const AABB& nmsSpace);
	Shader* mGridShader;
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