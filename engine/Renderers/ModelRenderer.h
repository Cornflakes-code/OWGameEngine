#pragma once
#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "RendererBase.h"
#include "../Helpers/Model.h"

class OWENGINE_API OWModelRenderer: public OWRenderer
{
public:
	AABB doSetup(const std::vector<OWMeshData>& meshes,
		const std::vector<OWModelData>& models) override;
	OWModelRenderer(const std::string& shaderFileName, unsigned int _vertexMode, unsigned int _vertexLocation)
		: OWRenderer(shaderFileName), mVertexMode(_vertexMode), mVertexLocation(_vertexLocation)
	{
		mIndicesMode = GL_TRIANGLES;
	}
	OWModelRenderer(Shader* shader, unsigned int _vertexMode, unsigned int _vertexLocation)
		: OWRenderer(shader), mVertexMode(_vertexMode), mVertexLocation(_vertexLocation)
	{
		mIndicesMode = GL_TRIANGLES;
	}
protected:
	void doRender() override;
	void prepareOpenGL();
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	OWModelData mData;
	unsigned int mIndicesMode = GL_INVALID_ENUM;
	unsigned int mVertexMode = GL_INVALID_ENUM;
	unsigned int mVertexLocation = GL_INVALID_ENUM;
	unsigned int mVao = 0;
	unsigned int mVbo = 0;
	unsigned int mEbo = 0;
#pragma warning( pop )
};
