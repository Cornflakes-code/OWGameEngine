#pragma once
#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "../Helpers/Mesh.h"
#include "RenderTypes.h"
#include "RendererBase.h"

class OWENGINE_API OWTextRenderer: public OWRenderer
{
public:
	OWTextRenderer(const std::string& shaderName, RenderType rt = DRAW_MULTI)
		: OWRenderer(shaderName, rt)
	{
	}
	OWTextRenderer(Shader* shader, RenderType rt = DRAW_MULTI)
		: OWRenderer(shader, rt)
	{
	}
	void doSetup(const std::vector<MeshData>& meshes,
		const std::vector<InstanceData>& instances,
		const std::vector<OWModelData>& models) override;
	void initText(const TextData& td, const glm::vec3& initialPosition);
protected:
	void add(const MeshData& meshData);
	void doRender() override;
	void prepareOpenGL();
#pragma warning( push )
#pragma warning( disable : 4251 )
	glm::vec4 mColour = glm::vec4();
	Texture mTexture;
	AABB mBounds;
	size_t mV4Size = 0;
	unsigned int mVao = 0;
	unsigned int mVbo = 0;
#pragma warning( pop )
private:
	void validate(const TextData* td) const;
};