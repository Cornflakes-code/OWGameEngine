#pragma once
#include <vector>
#include <string>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "OWComponent.h"


class OWENGINE_API OWMeshComponent : public OWMeshComponentBase
{
public:
	OWMeshComponent(OWActor* _owner, const std::string& _name);
	void add(const MeshData& meshData);
	void add(const std::vector<glm::vec3>& v);
	void add(const std::vector<glm::vec4>& v);
	virtual const std::vector<OWMeshData>& simpleMesh() const
	{
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