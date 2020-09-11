#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <Renderables/BoundingBox.h>

class Shader;
class TextBillboard;

class NoMansSky
{
	std::vector<glm::vec4> mStarPositions;
	std::vector<TextBillboard*> mStarLabels;
	std::vector<glm::vec4> mStarColours;
	std::vector<glm::vec3> mGrid;

	void loadStars(const std::string& fileName, 
				   const AABB& nmsSpace,
				   float scaleToWorld);
	void createGrid(const AABB& nmsSpace, 
					const glm::u32vec3& gridSizes,
					float scaleToWorld);
	Shader* mGridShader;
	Shader* mStarShader;

	GLuint mVao[2];
	GLuint mVboGridPos;
	GLuint mVboGridCol;
public:
	NoMansSky();
	void setUp(const std::string& fileName, const AABB& world);
	void render(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& model);
};