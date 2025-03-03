#pragma once
#include <vector>
#include <string>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "OWComponent.h"


class OWENGINE_API OWModelComponent : public OWMeshComponentBase
{
	std::vector<OWModelData> mData;
public:
	OWModelComponent(OWActor* _owner,
		const std::string& _name, const std::string& _modelName)
		: OWMeshComponentBase(_owner, _name) {
	}
	const std::vector<OWModelData>& complexMesh() const {
		return mData;
	}
protected:
	virtual void doSetup() override {}
};

