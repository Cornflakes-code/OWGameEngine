#pragma once

#include "../OWEngine/OWEngine.h"

#include "CommonUtils.h"
#include "UserInput.h"

class Scene;
class OcTree;
class Camera;
class OLDActor;
/*
	My implementation of a Fixed timestep physics/Variable timestep Render Game 
	Loop requires the separation of static and dynamic information. Class Scene 
	provides the base class for the static information and class ScenePhysicsState 
	provides the base class for the dynamic information. Inspection of the Game Loop
	in Movie.cpp will show that ScenePhysicsState is cloned and discarded
	many times per Render Loop so data should be minimised. Children of 
	Class ScenePhysicsState should be heavy on  functionality (lots of code) 
	but low on state information.
*/
class OWENGINE_API ScenePhysicsState
{
public:
	ScenePhysicsState(Scene* owner);
	virtual void setup() = 0;
	virtual void copy(ScenePhysicsState* source) = 0;
	virtual void fixedTimeStep(std::string& nextSceneName, OWUtils::Time::duration dt) = 0;
	virtual bool processUserCommands(const UserInput::AnyInput& userInput, 
									 std::string& nextScene, 
									 Camera* camera) = 0;
	virtual void variableTimeStep(OWUtils::Time::duration OW_UNUSED(dt)) {}

	// Good site for advanced interpolation (inclkuding eueler and RK4)
	// https://github.com/BryanCai/6.S096-Final-Project/blob/master/nbody-project/src/System.cpp
	virtual void interpolateRatio(const ScenePhysicsState* OW_UNUSED(previousState),
								  double OW_UNUSED(multPrev),
								  const ScenePhysicsState* OW_UNUSED(currentState),
								  double OW_UNUSED(multCurr)) {}
	virtual ScenePhysicsState* clone() = 0;
	Scene* owner() { return mOwner; }
	virtual void clear() {}
	OcTree* mOctTreeEx = nullptr;
	OLDActor* mSceneryEx = nullptr;
protected:

	// https://gafferongames.com/post/fix_your_timestep/
	float interpolate(float p1, float m1, float p2, float m2)
	{
		return p1 * m1 + p2 * m2;
	}
private:
	Scene* mOwner;
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

	void interpolateRenderTarget(OWUtils::Time::duration OW_UNUSED(totalSceneTime),
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
							 Camera* camera)
	{
		return current->processUserCommands(userInput, nextScene, camera);
	}
	ScenePhysicsState* current = nullptr;
	ScenePhysicsState* previous = nullptr;
	ScenePhysicsState* tempRenderTarget = nullptr;
private:
	bool mInterpolateAsVariableTimeStep = true;
};
