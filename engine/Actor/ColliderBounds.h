#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "../Actor/OWActor.h"
#include <glm/gtx/transform.hpp>

class TextComponent;

/*
* An OWActor to draw the bounding shapes of collideable types
* We create a set of Components for each OWCollider::CollisionType
* as well as a vector of OWCollider for each OWCollider::CollisionType
* The ord of the OWCollider::CollisionType is the index into the Components 
and also the index into the vector of vectors
*/
class OWENGINE_API OWColliderBounds: public OWActorDiscrete
{
public:
	OWColliderBounds(Scene* _scene, const std::string& _name);
	void addCollider(OWCollider* coll, OWActor* a, OWSize componentId);
	void initialise();
protected:
private:
	std::vector<std::vector<OWCollider*>> mCollidersGroupedByCollisionType;
};