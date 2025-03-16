#pragma once
#include <vector>

#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "../Geometry/BoundingBox.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shader Storage Buffer Object
struct SSBO
{
	std::vector<float> data;
	size_t nodeSize = 8; // Number of floats per node
	void append(const glm::vec4& p)
	{
		const float* f = glm::value_ptr(p);
		data.insert(data.end(), f, f + 4);
	}
	void append(float f)
	{
		data.push_back(f);
	}
};

class OWRenderData
{
public:
	std::vector<MeshData> meshes;
	std::vector<OWModelData> models;
	std::vector<InstanceData> instances;
	std::vector<Texture> textures;
	SSBO ssbo;
	
	void add(const OWRenderData& toAdd, bool purgeTextures = false);
	void convertMeshToInstance();
	AABB bounds() const;
};