#pragma once
#include <vector>
#include <string>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "Component.h"


class OWENGINE_API OWMeshComponent: public OWMeshComponentBase
{
public:
	OWMeshComponent(OWActor* _owner, const std::string& _name);
	OWMeshComponent* add(const MeshData& mesh)
	{
		mData.push_back(mesh);
		return this;
	}
	virtual const OWRenderData renderData(AABB& bounds) const override;
protected:
	virtual void doSetup() override;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::vector<MeshData> mData;
#pragma warning( pop )
};