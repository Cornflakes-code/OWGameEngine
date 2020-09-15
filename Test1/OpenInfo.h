#pragma once

#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "Shader.h"
#include "Texture.h"
#include "OpenGLObject.h"

struct OpenInfoObject:public OpenGLObject
{
	Shader shader;
	Texture texture;

	unsigned int vao = 0;
	unsigned int vbo = 0;
	unsigned int ebo = 0;
	std::vector<std::vector<float>> triangles;
	std::vector<unsigned int> indices;
	std::vector<std::function<void(int)>> attrPointers;
	unsigned int size() const;
	void use() override;
	void cleanup() override;
	void configure() override;
private:
};

//struct OpenInfoTextureObject :public OpenGLObject
//{
//	Texture texture;
//	unsigned int vao = 0;
//	unsigned int vbo = 0;
//	std::vector<std::vector<float>> triangles;
//	std::vector<std::function<void(int)>> attrPointers;
//	unsigned int size() const;
//	void use() override;
//	void cleanup() override;
//	void configure() override;
//private:
//};

struct OpenInfo
{
	std::vector<std::unique_ptr<OpenGLObject>> objects;
	void polygonDrawMode(int face, int mode);
	void clearColour(int buffer, float red, float green, float blue, float alpha);
	void use();
	void cleanup();
};
