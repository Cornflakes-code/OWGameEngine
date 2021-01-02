#pragma once

#include <vector>
#include <functional>
#include <memory>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "../Helpers/MeshData.h"
#include "../Helpers/ErrorHandling.h"
#include "RenderableBase.h"

class Shader;
class MeshRenderer;
struct aiMesh;
struct aiScene;
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
	Mesh(const glm::vec3& _initialPosition = glm::vec3(0.0f, 0.0f, 0.0f))
		: RenderableBase(_initialPosition) {}
	void create(aiMesh *mesh, const aiScene *scene);
	void makeDeep();
	void initx(const std::vector<MeshData::Vertex>& vertices,
		const std::vector<unsigned int>& indices, bool _calcNormals);
	void renderer(MeshRenderer* source);
	const MeshRenderer* renderer() const { return mRenderer; }
	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const MoveController* mover = nullptr,
		OWUtils::RenderCallbackType renderCb = nullptr,
		OWUtils::ResizeCallbackType resizeCb = nullptr) const override;

	const std::vector<MeshData::Vertex>& vertices() const { return mData.vertices; }
	const std::vector<unsigned int>& indices() const { return mData.indices; }
	unsigned int indiceMode() const { return mData.indicesMode;  }
	unsigned int vertexMode() const { return mData.vertexMode; }

	void addTexture(const Texture& _texture)
	{
		// If there are more than one texture unit in a shader then we need to consider
		// GL_TEXTURE1, GL_TEXTURE2 and so on. See:
		// https://opentk.net/learn/chapter1/5-multiple-textures.html
		mData.textures.push_back(_texture);
	}
	const std::vector<Texture> textures() const { return mData.textures;  }
protected:
	AABB calcBounds() const override;

#pragma warning( push )
#pragma warning( disable : 4251 )
	MeshRenderer* mRenderer = nullptr;
	MeshData mData;
	//	glm::vec4 mColour;
//	std::string mColourName;
#pragma warning( pop )
	friend class ModelFactory;
	friend class MeshRenderer;
};
