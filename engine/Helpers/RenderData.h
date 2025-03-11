#pragma once
#include <vector>

#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "../Geometry/BoundingBox.h"

class OWRenderData
{
public:
	std::vector<MeshData> meshes;
	std::vector<OWModelData> models;
	std::vector<InstanceData> instances;
	std::vector<Texture> textures;

	void add(const OWRenderData& toAdd);
	AABB bounds() const;
};