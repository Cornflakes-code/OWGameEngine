#pragma once
#include <vector>
#include <string>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "Component.h"


class OWENGINE_API OWModelComponent: public OWMeshComponentBase
{
	std::vector<OWModelData> mData;
public:
	OWModelComponent(OWActor* _owner,
		const std::string& _name, const std::string& _modelName);
	void add(const OWModelData& md);
	const OWRenderData renderData(AABB& bounds) const override;
protected:
	virtual void doSetup() override;
};

