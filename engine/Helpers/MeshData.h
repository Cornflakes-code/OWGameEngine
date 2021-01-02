#pragma once
#include <vector>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../Helpers/Texture.h"


struct MeshData
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoord;
		glm::vec3 tangent;
		glm::vec3 biTangent;
		Vertex() {}

		Vertex(const glm::vec3& _position,
			const glm::vec3& _normal = glm::vec3(0),
			const glm::vec2& _texCoord = glm::vec2(0))
			: position(_position),
			normal(_normal),
			textureCoord(_texCoord)
		{}
	};

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int indicesMode = GL_INVALID_ENUM;
	unsigned int vertexMode = GL_TRIANGLES;
	void calcNormals();
};

