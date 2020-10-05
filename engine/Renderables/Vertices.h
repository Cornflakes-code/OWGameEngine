#pragma once
#include <vector>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "BoundingBox.h"

class MoveController;

class OWENGINE_API Vertices
{
public:
	Vertices(const glm::vec3& _initialPosition);
	virtual ~Vertices() {}
	AABB bounds() const 
	{ 
		if (!mBoundsValid)
		{
			mBounds = calcBounds();
			mBoundsValid = true;
		}
		return mBounds; 
	}
	glm::vec3 initialPosition() const { return mInitialPosition; }

	void shader(Shader* newValue, const std::string& pvmName)
	{
		mShader = newValue;
		mPVMName = pvmName;
	}

	void vertices(const std::vector<glm::vec3>& v,
					unsigned int location,
					unsigned int drawMode);
	void vertices(const std::vector<glm::vec4>& v,
					unsigned int location,
					unsigned int drawMode);
	virtual void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const MoveController* mover = nullptr,
		OWUtils::RenderCallbackType renderCb = nullptr,
		OWUtils::ResizeCallbackType resizeCb = nullptr) const = 0;
protected:
#pragma warning( push )
#pragma warning( disable : 4251 )
	Shader* mShader = nullptr;
	std::string mPVMName;
	const glm::vec3 mInitialPosition;
	std::vector<glm::vec3> mVec3;
	std::vector<glm::vec4> mVec4;
	unsigned int mVertexLoc = GL_INVALID_INDEX;
	unsigned int mVertexMode = GL_INVALID_ENUM;
	OWUtils::RenderCallbackType mRenderCallback = nullptr;
	OWUtils::ResizeCallbackType mResizeCallback = nullptr;
#pragma warning( pop )
private:
	AABB calcBounds() const;
#pragma warning( push )
#pragma warning( disable : 4251 )
	mutable AABB mBounds;
	mutable bool mBoundsValid = false;
#pragma warning( pop )
	friend class VertexRenderBase;
};