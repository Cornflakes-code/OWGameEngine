#include "OWComponent.h"
#include "../Core/ErrorHandling.h"

static std::vector<OWMeshData> gEmptyMeshes;

const std::vector<OWMeshData>& OWMeshComponentBase::simpleMesh() const
{
	return gEmptyMeshes;
}

static std::vector<OWModelData> gEmptyModels;
const std::vector<OWModelData>& OWMeshComponentBase::complexMesh() const
{
	return gEmptyModels;
}

void OWComponent::name(const std::string& _name)
{
	if (mName.size() != 0)
		throw NMSLogicException(std::stringstream()
			<< "OWComponent ["
			<< mName << "] is incorrectly trying to be renamed to ["
			<< _name << "[\n");
	mName = _name;
}
