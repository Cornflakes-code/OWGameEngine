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
	OWMeshComponent* setData(const MeshData& mesh);
	virtual const OWRenderData renderData(AABB& bounds) override;
protected:
	virtual void doSetup() override;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	MeshData mData;
#pragma warning( pop )
};