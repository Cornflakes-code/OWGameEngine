#include "ModelComponent.h"

#include <Helpers/ModelFactory.h>

OWModelComponent::OWModelComponent(OWActor* _owner,
	const std::string& _name, const std::string& _modelName)
	: OWMeshComponentBase(_owner, _name, OWRenderTypes::DrawType::ThreeD)
{
	OWModelData md = ModelFactory().create(_modelName, false).children[0].meshes[0];
	mData.push_back(md);
}

void OWModelComponent::add(const OWModelData& md) 
{ 
	mData.push_back(md); 
}

const OWRenderData OWModelComponent::renderData(AABB& bounds) const
{
	OWRenderData rd;
	for (const auto& elm : mData)
	{
		bounds = bounds | elm.bounds();
		rd.models.push_back(elm);
	}
	return rd;
}

void OWModelComponent::doSetup() 
{

}
