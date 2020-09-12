#pragma once

#include "UserInput.h"

#include "../Helpers/CommonUtils.h"

class Scene;
class Camera;
/*
	My implementation of a Fixed timestep physics/Variable timestep Render Game 
	Loop requires the separation of static and dynamic information. Class Scene 
	provides the base class for the static information and class ScenePhysicsState 
	provides the base class for the dynamic information. Inspection of the Game Loop
	in Movie.cpp will show that ScenePhysicsState is cloned and discarded
	many times per Render Loop. Children of Class ScenePhysicsState will be heavy on 
	functionality (lots of code) but low on state information.
*/
class ScenePhysicsState
{
public:
	ScenePhysicsState(const Scene* owner)
		: mOwner(owner)
	{}
	virtual void copy(ScenePhysicsState* source) = 0;
	virtual void fixedTimeStep(std::string& nextSceneName, OWUtils::Time::duration dt) = 0;
	virtual bool processUserCommands(const UserInput::AnyInput& userInput, std::string& nextScene, 
									 const glm::uvec2& screenSize, Camera* camera) = 0;
	virtual void variableTimeStep(OWUtils::Time::duration dt) {}

	// Good site for advanced interpolation (inclkuding eueler and RK4)
	// https://github.com/BryanCai/6.S096-Final-Project/blob/master/nbody-project/src/System.cpp
	virtual void interpolateRatio(const ScenePhysicsState* previousState, double multPrev,
		const ScenePhysicsState* currentState, double multCurr) {}
	virtual ScenePhysicsState* clone() = 0;
	const Scene* owner() const { return mOwner; }
	virtual void clear() {}
protected:
	// https://gafferongames.com/post/fix_your_timestep/
	float interpolate(float p1, float m1, float p2, float m2)
	{
		return p1 * m1 + p2 * m2;
	}
private:
	const Scene* mOwner;
};

class SceneLogic
{
public:
	void clear()
	{
		previous->clear();
		current->clear();
		tempRenderTarget->clear();
	}
	
	void copyCurrentToPrevious()
	{
		previous->copy(current);
	}

	void fixedUpdate(std::string& nextScene, OWUtils::Time::duration fixedStep)
	{
		current->fixedTimeStep(nextScene, fixedStep);
	}

	ScenePhysicsState* renderTarget()
	{
		return tempRenderTarget;
	}

	void interpolateRenderTarget(OWUtils::Time::duration totalSceneTime, 
								 OWUtils::Time::duration remainingTime, 
								 OWUtils::Time::duration fixedStep)
	{
		if (mInterpolateAsVariableTimeStep)
		{
			tempRenderTarget->copy(previous);
			tempRenderTarget->variableTimeStep(remainingTime);
		}
		else
		{
			double alpha = remainingTime.count() / static_cast<double>(fixedStep.count());
			tempRenderTarget->interpolateRatio(previous, 1.0 - alpha, current, alpha);
		}
	}
	bool processUserCommands(const UserInput::AnyInput& userInput, std::string& nextScene, 
							 const glm::uvec2& screenSize, Camera* camera)
	{
		return current->processUserCommands(userInput, nextScene, screenSize, camera);
	}
	ScenePhysicsState* current;
	ScenePhysicsState* previous;
	ScenePhysicsState* tempRenderTarget;
private:
	bool mInterpolateAsVariableTimeStep = true;
};
