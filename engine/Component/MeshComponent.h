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
	OWMeshComponent* add(const InstanceData& instanceData);
	OWMeshComponent* add(const MeshData& meshData);
	OWMeshComponent* add(const std::vector<glm::vec3>& v);
	OWMeshComponent* add(const std::vector<glm::vec4>& v);
	virtual const std::vector<OWMeshData> simpleMesh(AABB& bounds) const
	{
		AABB b;
		for (const OWMeshData& md : mData)
		{
			b = b | md.bounds();
		}
		bounds = b;
		return mData;
	}
protected:
	virtual void doSetup() override {}
private:
	void validate(const MeshData& mdl);
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::vector<OWMeshData> mData;
#pragma warning( pop )
};