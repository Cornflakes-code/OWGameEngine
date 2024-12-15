#pragma once

#include <vector>
#include <string>

//#include "MovableObject.h"
//#include "ErrorHandling.h"
//#include "BoundingBox.h"
#include "SceneGraphNode.h"
#include "Particle.h"

class OWENGINE_API Actor: public SceneGraphNode
{
private:
	Physical* mPhysical;
public:
	/// 0 = The object did not move.
	/// 1 = the object was moved.
	/// 2 = the object just died.
	virtual int tick(float dt) override
	{ 
		mPhysical->move(dt);
		return false; 
	}
	Physical* physical() { return mPhysical; }
	const glm::vec3& position() const { return mPhysical->position(); }
	const AABB& bounds() const { return mPhysical->bounds(); }
protected:
	Actor(Physical* _physical, Actor* _parent = nullptr)
		: SceneGraphNode(_parent), mPhysical(_physical) {}
	void bounds(const AABB& value) { mPhysical->bounds(value); }
	void position(const glm::vec3& value) { mPhysical->position(value); }
	virtual void attachObject(MovableObject*) {}
};
