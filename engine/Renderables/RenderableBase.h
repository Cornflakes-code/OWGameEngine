#pragma once
#include <string>

#include "../OWEngine/OWEngine.h"

#include "BoundingBox.h"

class Shader;
class MoveController;

class OWENGINE_API RenderableBase
{
public:
	RenderableBase(const glm::vec3& _initialPosition)
		: mInitialPosition(_initialPosition) {}
	glm::vec3 initialPosition() const { return mInitialPosition; }
	AABB bounds() const
	{
		if (!mBoundsValid)
		{
			mBounds = calcBounds();
			mBoundsValid = true;
		}
		return mBounds;
	}
	const Shader* shader() const { return mShader; }
	std::string pvmName() const { return mPVMName; }
	bool includeModelInPVM() const 
	{ 
		// Testing for name.size() is seriously tacky.
		// Must be a better way !!
		return mPVMName.size() < 3;
	}
	void shader(Shader* newValue, const std::string& pvmName)
	{
		mShader = newValue;
		mPVMName = pvmName;
	}
	virtual void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const MoveController* mover = nullptr,
		OWUtils::RenderCallbackType renderCb = nullptr,
		OWUtils::ResizeCallbackType resizeCb = nullptr) const = 0;
protected:
	void invalidateBounds() const { mBoundsValid = false; }
#pragma warning( push )
#pragma warning( disable : 4251 )
	Shader* mShader = nullptr;
	std::string mPVMName;
	OWUtils::RenderCallbackType mRenderCallback = nullptr;
	OWUtils::ResizeCallbackType mResizeCallback = nullptr;
	const glm::vec3 mInitialPosition;
#pragma warning( pop )
	virtual AABB calcBounds() const = 0;
	friend class RenderBase;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	mutable AABB mBounds;
	mutable bool mBoundsValid = false;
#pragma warning( pop )

};