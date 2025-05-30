#pragma once
#include <vector>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "Texture.h"
#include <Geometry/BoundingBox.h>

struct aiMesh;
struct aiScene;

struct OWModelData
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 textureCoord;
		glm::vec3 normal;
		//glm::vec3 tangent;
		//glm::vec3 biTangent;
		Vertex()
			: position(glm::vec3(0)),
			normal(glm::vec3(0)),
			textureCoord(glm::vec2(0))
		{
		}

		Vertex(const glm::vec3& _position,
			const glm::vec3& _normal = glm::vec3(0),
			const glm::vec2& _texCoord = glm::vec2(0))
			: position(_position),
			normal(_normal),
			textureCoord(_texCoord)
		{
		}
	};

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	void create(aiMesh* mesh, const aiScene* scene);
	void calcNormals();
	AABB bounds() const;
};


struct OWModelDataEx
{
	std::vector<OWModelDataEx> children;
	std::vector<OWModelData> meshes;
};
