#pragma once
#include <vector>
#include <functional>
#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

class Shader;

/*
	Basically a struct that contains the data to be rendered.
	Passed to VertexRenderer to be rendered.
	Can be used as is or as a base class for more complex graphics.
	No getters. Values only accessible by VertexRenderer and derived classes.
	Does this means it cannot be inherited from outside of the DLL?
*/
class OWENGINE_API VertexSourceCompound
{
public:
	VertexSourceCompound();
	virtual void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model) const = 0;

protected:
#pragma warning( push )
#pragma warning( disable : 4251 )
	//friend class VertexRendererCompound;
#pragma warning( pop )
};