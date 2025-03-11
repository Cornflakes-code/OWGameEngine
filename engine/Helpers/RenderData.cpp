#include "RenderData.h"

void OWRenderData::add(const OWRenderData& toAdd)
{
	meshes.insert(meshes.end(), toAdd.meshes.begin(), toAdd.meshes.end());
	models.insert(models.end(), toAdd.models.begin(), toAdd.models.end());
	instances.insert(instances.end(), toAdd.instances.begin(), toAdd.instances.end());
	textures.insert(textures.end(), toAdd.textures.begin(), toAdd.textures.end());
}

AABB OWRenderData::bounds() const
{
	AABB retval = AABB(0);
	for (const auto& m : meshes)
	{
		retval = retval | m.bounds();
	}
	for (const auto& m : models)
	{
		retval = retval | m.bounds();
	}
	for (const auto& m : instances)
	{
		retval = retval | m.bounds();
	}

	return retval;
}
