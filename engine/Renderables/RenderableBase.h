#pragma once
#include <string>

#include "../OWEngine/OWEngine.h"

#include "BoundingBox.h"

class Shader;
class MoveController;

class OWENGINE_API RenderableBase
{
public:
	AABB bounds() const
	{
		if (!mBoundsValid)
		{
			mBounds = calcBounds();
			mBoundsValid = true;
		}
		return mBounds;
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
#pragma warning( pop )
	virtual AABB calcBounds() const = 0;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	mutable AABB mBounds;
	mutable bool mBoundsValid = false;
#pragma warning( pop )

};