#pragma once
#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "VertexRenderBase.h"

class InstanceSource;

class OWENGINE_API InstanceSourceRenderer: public VertexRenderBase
{
public:
	InstanceSourceRenderer();
	void prepare(const InstanceSource* source);
	void render(const InstanceSource* source,
		const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		OWUtils::RenderCallbackType renderCb = nullptr,
		OWUtils::ResizeCallbackType resizeCb = nullptr) const;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	// mVbo[0] The VBO containing the triangles to draw
	// mVbo[1] The VBO containing the positions of the particles
	// mVbo[2] The VBO containing the colors of the particles
	unsigned int mVbo[3] = { 0, 0, 0 };
	unsigned int mVao = 0;

	virtual void checkSourceForErrors(const InstanceSource* source);
#pragma warning( pop )
};