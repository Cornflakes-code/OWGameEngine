#include "RenderData.h"

void OWRenderData::add(const OWRenderData& toAdd, bool purgeTextures)
{
	meshes.insert(meshes.end(), toAdd.meshes.begin(), toAdd.meshes.end());
	models.insert(models.end(), toAdd.models.begin(), toAdd.models.end());
	instances.insert(instances.end(), toAdd.instances.begin(), toAdd.instances.end());
	if (purgeTextures)
	{
		// Remove identical textures
		size_t startNdx = 0;
		if (!textures.size() && toAdd.textures.size())
		{
			textures.push_back(toAdd.textures[startNdx++]);
		}
		for (size_t i = startNdx; i < toAdd.textures.size(); i++)
		{
			const Texture& tex = toAdd.textures[i];
			if (tex.samplerName() == textures.back().samplerName())
				continue;
			textures.push_back(tex);
		}
	}
	else
	{
		textures.insert(textures.end(), toAdd.textures.begin(), toAdd.textures.end());
	}
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
