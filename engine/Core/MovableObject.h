#pragma once

#include "../OWEngine/OWEngine.h"
#include "BoundingBox.h"

class MovableObject
{
	MovableObject();
};

/*

class OWENGINE_API Object
{
public:
	enum TickType { InitialTick, FinalTick};
	virtual void tick(float dt, TickType tt = InitialTick) = 0;
};

class Scene;
class OWENGINE_API Actor : public Object
{
	Scene* mScene;
public:
	Actor(Scene* _scene)
		:mScene(_scene) 
	{}

	std::vector<Component*> Components;
	std::vector<SceneComponent*> SceneComponents; // Tree
	void tick(float dt, TickType tt = InitialTick) override
	{
		for (auto& c : Components)
			c->tick(dt);
		for (auto& c : SceneComponents)
			c->tick(dt);
	}
	
	void collideHandled(MovableComponent* _ourComponent, Actor* other, MovableComponent* otherComponent)
	{
		// returning false means the movable objects deals with it. I assume they just rebound
		return false;
	}
protected:
	virtual bool canCollide(MovableComponent* _ourComponent, Actor* other, MovableComponent* otherComponent)
	{
		// for example our thigh and shin of same leg may interesect but they cannot collide
		// but hands of different arms can.
		return true;
	}
};

class OWENGINE_API Component : public Object
{
protected:
	Actor* mOwner;
public:
	Component(Actor* _owner)
		: mOwner(_owner) {}
};

class OWENGINE_API MovableComponent: public Component
{
	AABB mBounds;
	// Position, velocity etc data
public:
	MovableComponent(Actor* _owner)
	: Component(_owner)
	{}
	virtual const AABB& bounds() = 0;
	virtual bool canCollide(MovableComponent* other)
	{
		// called before testing for couldCollide
		return mOwner->canCollide(this, other->mOwner, other);
	}

	virtual bool couldCollide(MovableComponent* other)
	{
		// if this is called then we know that AABB's of these objects intersect.
		if (fineGrainCollide(other))
		{
			// We really did collide
			// Allow the actors to do something
			if (!mOwner->collideHandled(this, other->mOwner, other))
			{
				// here we update positions
			}
		}
	}

	virtual bool fineGrainCollide(MovableComponent* other)
	{
		return false;
	}

	void tick(float dt, TickType tt = InitialTick) override
	{
		// adjust position
	}
};

class OWENGINE_API SceneComponent: public MovableComponent
{
	RendererBase* mRenderer;
public:
	SceneComponent(Actor* _owner)
	:MovableComponent(_owner) {}

	void tick(float dt, TickType tt = InitialTick) override
	{
		MovableComponent::tick(dt);
		// adjust renderers with new positions
	}
	virtual void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos) = 0;
}; 

*/
