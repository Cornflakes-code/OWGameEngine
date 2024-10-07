#pragma once
#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "RendererBaseShader.h"

class Particles;
struct MeshDataHeavy;

class OWENGINE_API HeavyRenderer: public RendererBaseShader
{
public:
	HeavyRenderer(Shader* shader)
		: RendererBaseShader(shader) {}
	void setup(MeshDataHeavy* data, unsigned int vertexMode, 
			unsigned int vertexLocation = 0);

	virtual void doRender() const override;
protected:
	void validate() const;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	MeshDataHeavy* mData;
	unsigned int mIndicesMode = GL_INVALID_ENUM;
	unsigned int mVertexMode = GL_INVALID_ENUM;
	unsigned int mVertexLocation = GL_INVALID_ENUM;
	unsigned int mVao = 0;
	unsigned int mVbo = 0;
	unsigned int mEbo = 0;
#pragma warning( pop )
};