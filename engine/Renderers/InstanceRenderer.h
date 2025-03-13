#pragma once

#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "RendererBase.h"
#include "../Geometry/BoundingBox.h"
#include "../Helpers/Mesh.h"

class OWENGINE_API OWInstanceRenderer: public OWRenderer
{
public:
	OWInstanceRenderer(const std::string& shaderName, RenderType rt = DRAW_MULTI)
		: OWRenderer(shaderName, rt) {
	}
	OWInstanceRenderer(Shader* _shader, RenderType rt = DRAW_MULTI)
		: OWRenderer(_shader, rt) {
	}
protected:
	void doSetup(const OWRenderData& renderData) override;
	void doRender() override;
private:
	void setupMesh(const MeshData& mesh);
	void setupInstance();
	void setupModel(const OWModelData& model);
#pragma warning( push )
#pragma warning( disable : 4251 )
	InstanceData mData;
	// Keep the count seperate cos we could blow away mData.v3/v4
	GLsizei mVerticeCount = 0;
	GLsizei mPositionCount = 0;
	GLsizei mColourCount = 0;
	// mVbo[0] The VBO containing the triangles to draw
	// mVbo[1] The VBO containing the positions of the particles
	// mVbo[2] The VBO containing the colors of the particles
	unsigned int mVao = 0;
	unsigned int mVbo[3] = { 0, 0, 0 };
	unsigned int mEbo = 0;
#pragma warning( pop )
};
