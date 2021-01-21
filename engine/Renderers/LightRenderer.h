#pragma once
#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "../Helpers/MeshDataLight.h"

#include "RendererBase.h"

class OWENGINE_API LightRenderer : public RendererBase
{
public:
	LightRenderer(Shader* shader, const std::string& pvm)
		: RendererBase(shader, pvm) {}
	void setup(const MeshDataLight* meshData, const std::string& colourName);
	void setup(const std::vector<glm::vec3>& v,
		unsigned int vertexMode, unsigned int vertexLocation = 0);
	void setup(const std::vector<glm::vec4>& v,
		unsigned int vertexMode, unsigned int vertexLocation = 0);
	virtual void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const MoveController* mover = nullptr,
		RenderCallbackType renderCb = nullptr,
		ResizeCallbackType resizeCb = nullptr) const override;
private:
	void validate(const MeshDataLight* mdl);
#pragma warning( push )
#pragma warning( disable : 4251 )
	MeshDataLight::RenderData mData;
	// mVbo[0] The VBO containing the triangles to draw
	// mVbo[1] The VBO containing the positions of the particles
	// mVbo[2] The VBO containing the colors of the particles
	unsigned int mVao = 0;
	unsigned int mVbo = 0;
	unsigned int mEbo = 0;	
#pragma warning( pop )
};