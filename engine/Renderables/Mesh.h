#pragma once

#include <vector>
#include <functional>
#include <memory>


#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "../Helpers/Texture.h"
#include "../Helpers/ErrorHandling.h"
#include "RenderableBase.h"

class Shader;
class ModelRenderer;
/*
	Basically a struct that contains the data to be rendered.
	Passed to VertexRenderer to be rendered.
	Can be used as is or as a base class for more complex graphics.
	No getters. Values only accessible by VertexRenderer and derived classes.
	Does this means it cannot be inherited from outside of the DLL?
*/
class OWENGINE_API Mesh : public RenderableBase
{
public:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoord;

		Vertex(const glm::vec3& _position,
			const glm::vec3& _normal = glm::vec3(0),
			const glm::vec2& _texCoord = glm::vec2(0))
		:	position(_position),
			normal(_normal),
			textureCoord(_texCoord) 
		{}
	};

	Mesh(const glm::vec3& _initialPosition = glm::vec3(0.0f, 0.0f, 0.0f))
		: RenderableBase(_initialPosition) {}
	void readFiles(const std::string& modelFileName,
		const std::string& textureFileName = std::string(""),
		bool cache = true);	
	void makeDeep();
	void init(const std::vector<Mesh::Vertex>& vertices,
		const std::vector<unsigned int>& indices, bool _calcNormals);
	void addRenderer(ModelRenderer* source);
	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const MoveController* mover = nullptr,
		OWUtils::RenderCallbackType renderCb = nullptr,
		OWUtils::ResizeCallbackType resizeCb = nullptr) const override;

	const std::vector<Mesh::Vertex>& vertices() const { return mData->vertices; }
	const std::vector<unsigned int>& indices() const { return mData->indices; }
	unsigned int indiceMode() const { return mData->indicesMode;  }
	unsigned int vertexMode() const { return mData->vertexMode; }

	void texture(const Texture& _texture)
	{
		// If there are more than one texture unit in a shader then we need to consider
		// GL_TEXTURE1, GL_TEXTURE2 and so on. See:
		// https://opentk.net/learn/chapter1/5-multiple-textures.html
		mData->textures.push_back(_texture);
	}
	const std::vector<Texture> textures() const { return mData->textures;  }
protected:
	struct ModelData
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		unsigned int indicesMode = GL_INVALID_ENUM;
		unsigned int vertexMode = GL_TRIANGLES;
	};
	void calcNormals(ModelData* md) const;
	AABB calcBounds() const override;

#pragma warning( push )
#pragma warning( disable : 4251 )
	ModelRenderer* mRenderer = nullptr;
	std::shared_ptr<ModelData> mData;
	//	glm::vec4 mColour;
//	std::string mColourName;
#pragma warning( pop )
	friend class ModelFactory;
	friend class ModelRenderer;
};
