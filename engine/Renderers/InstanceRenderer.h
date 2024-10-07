#pragma once

#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "../Helpers/MeshDataInstance.h"

#include "RendererBaseShader.h"

class OWENGINE_API InstanceRenderer: public RendererBaseShader
{
public:
	InstanceRenderer(Shader* shader)
		: RendererBaseShader(shader) {}
	void setup(const MeshDataInstance* meshData);
	void doRender() const override;
private:
	void validate(const MeshDataInstance* meshData) const;
#pragma warning( push )
#pragma warning( disable : 4251 )
	MeshDataInstance::RenderData mData;
	// mVbo[0] The VBO containing the triangles to draw
	// mVbo[1] The VBO containing the positions of the particles
	// mVbo[2] The VBO containing the colors of the particles
	unsigned int mVao = 0;
	unsigned int mVbo[3] = { 0, 0, 0 };
	unsigned int mEbo = 0;
#pragma warning( pop )
};