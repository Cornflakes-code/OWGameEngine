#pragma once
#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "VerticesRenderer.h"

class Particles;

class OWENGINE_API ParticlesRenderer: public VerticesRenderer
{
public:
	ParticlesRenderer();
	void prepare(const Particles* source);
	virtual void checkSourceForErrors(const Particles* source) const;
	void render(const Particles* source,
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
#pragma warning( pop )
};