#include "MeshData.h"

void MeshData::calcNormals()
{
#ifdef MESH_HAS_NORMAL
	for (int i = 0; i < indices.size(); i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		glm::vec3 v1 = vertices[i1].position - vertices[i0].position;
		glm::vec3 v2 = vertices[i2].position - vertices[i0].position;
		glm::vec3 v2Normal = glm::normalize(v2);
		glm::vec3  normal = glm::cross(v1, v2Normal);

		vertices[i0].normal += normal;
		vertices[i1].normal += normal;
		vertices[i2].normal += normal;
	}

	for (int i = 0; i < vertices.size(); i++)
		vertices[i].normal = glm::normalize(vertices[i].normal);
#endif
}
