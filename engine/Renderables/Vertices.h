#pragma once
#include <vector>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "RenderableBase.h"

class OWENGINE_API Vertices: public RenderableBase
{
public:
	Vertices(const glm::vec3& _initialPosition);
	virtual ~Vertices() {}

	void vertices(const std::vector<glm::vec3>& v,
					unsigned int location,
					unsigned int drawMode);
	void vertices(const std::vector<glm::vec4>& v,
					unsigned int location,
					unsigned int drawMode);
protected:
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::vector<glm::vec3> mVec3;
	std::vector<glm::vec4> mVec4;
	unsigned int mVertexLoc = GL_INVALID_INDEX;
	unsigned int mVertexMode = GL_INVALID_ENUM;
	AABB calcBounds() const override;
#pragma warning( pop )
private:
	friend class VerticesRenderer;
};