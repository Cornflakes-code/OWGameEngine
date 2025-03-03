#pragma once

#include <string>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "../Core/OWObject.h"
#include "../Renderers/RenderTypes.h"
#include "../Helpers/Mesh.h"
#include "../Helpers/Model.h"

class OWActor;
class OWComponent
{
public:
	OWComponent(OWActor* _owner, const std::string& _name)
		:mOwner(_owner), mName(_name) {
	}
	void actor(OWActor* newValue) { mOwner = newValue; }
	const OWActor* actor() const { return mOwner; }
	bool active() const { return mActive; }
	void active(bool newValue) { mActive = newValue; }
	std::string name() const { return mName; }
	void name(const std::string& _name);
	void setup()
	{
		if (!mSetup)
		{
			doSetup();
			mSetup = true;
		}
	}
protected:
	virtual void doSetup() = 0;
private:
	OWActor* mOwner;
	bool mSetup = false;
	bool mActive = false;
	std::string mName;
};

struct OWModelData;

class OWMeshComponentBase: public OWComponent
{
	RenderTypes::ShaderMutator mMutator;
public:
	OWMeshComponentBase(OWActor* _owner, const std::string& _name)
		: OWComponent(_owner, _name) {
	}
	RenderTypes::ShaderMutator mutator() const {
		return mMutator;
	}
	void mutator(RenderTypes::ShaderMutator newValue) {
		mMutator = newValue;
	}
	virtual const std::vector<OWMeshData>& simpleMesh() const;
	virtual const std::vector<OWModelData>& complexMesh() const;
};
