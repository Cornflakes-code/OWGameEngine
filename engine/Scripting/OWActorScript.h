#pragma once
#include <string>
#include <limits>
#include <list> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../OWEngine/OWEngine.h"
#include "../Component/PhysicalComponent.h"


class OWActor;
class OWPhysics;
class OWMeshComponentBase;
class OWTransform;
class OWSoundComponent;

class OWScriptCommmand
{
	enum class Subject
	{
		Position, Velocity, Acceleration, AngularVelocity, AngularAcceleration, Mass, Hardness, Visibility, NoSubject
	};
	enum class Operation
	{
		Set, IncreaseTo,
	};
	Subject mNoun = Subject::NoSubject;
	Operation mOp = Operation::Set;
	float* mValue = nullptr;
	float* mConditionValue = nullptr;
	unsigned int mTarget = 0;
	void execute();
protected:
	OWScriptCommmand(unsigned int _target) : mTarget(_target) {}
	void setCommand(Subject _noun, Operation _op, float*& _value, float*& _conditionValue)
	{
		mNoun = _noun;
		mOp = _op;
		mValue = _value;
		mConditionValue = _conditionValue;
	}
};

/*
// This command allows float properties to be set to absolute values
class OWScriptAbsoluteCommmand1F: public OWScriptCommmand
{
	enum class Subject
	{
		Mass, Hardness, Visibility
	};
public:
	OWScriptAbsoluteCommmand1F() {}
};

// This command allows vector properties to be set to absolute values
class OWScriptAbsoluteCommmand3F: public OWScriptCommmand
{
	enum class Subject
	{
		Position, Velocity, Acceleration, AngularVelocity, AngularAcceleration
	};
public:
	OWScriptAbsoluteCommmand3F() {}
};

// This command allows float properties to be increased/decreased
class OWScriptRelativeCommmand1F: public OWScriptCommmand
{
	enum class Subject
	{
		Velocity, Acceleration, AngularVelocity, AngularAcceleration
	};
	Subject mSubject;
public:
	OWScriptRelativeCommmand1F() {}
};

// This command allows vector properties to be increased/decreased
class OWScriptRelativeCommmand3F: public OWScriptCommmand
{
	enum class Subject
	{
		Velocity, Acceleration, AngularVelocity, AngularAcceleration
	};
	Subject mSubject;
public:
	OWScriptRelativeCommmand3F() {}
};
*/
class OWENGINE_API OWScriptComponent
{
	OWActor* mActor = nullptr;
	std::list< OWScriptCommmand*> mCommands;
public:
	struct OWENGINE_API RequiredComponents
	{
		OWPhysics* phys = nullptr;
		OWMeshComponentBase* mesh = nullptr;
		OWTransform* trans = nullptr;
		OWSoundComponent* sound = nullptr;
	};
	OWScriptComponent(const std::string& fileName);
	OWScriptComponent();
	void actor(OWActor* host) {
		mActor = host;
	}
	const OWActor* actor() const {
		return mActor;
	}
	void addCommand(OWScriptCommmand* command) {
		mCommands.push_front(command);
	}
	void tick(float dt);
};
