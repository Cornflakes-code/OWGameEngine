#include "RopeNormaliser.h"
#include <Core/ErrorHandling.h>

std::vector<unsigned int> RopeNormaliser::mIndexBuffer;
RopeNormaliser::IndexTriangle::IndexTriangle(unsigned int a, unsigned int b, unsigned int c, const std::vector<glm::vec3>& points)
{
	indicies[0] = a;
	indicies[1] = b;
	indicies[2] = c;

	glm::vec3 edge1 = points[b] - points[a];
	glm::vec3 edge2 = points[c] - points[a];
	glm::vec3 t = glm::cross(edge1, edge2);
	normal = glm::normalize(t);
	if ((std::isnan(normal.x)) || 
		(std::isnan(normal.y)) || 
		(std::isnan(normal.z)))
	{
		//		normal.x = 0.10f; normal.y = 0.10f; normal.z = 0.10f;
		throw NMSLogicException("A normal is a NAN");
	}
}

void RopeNormaliser::aggregateFaces(unsigned int ndx, const IndexTriangle& tri)
{
	TriAngleElements::iterator iter = triangles.find(ndx);
	if (iter == triangles.end())
		triangles.insert({ ndx, {tri} });
	else
		iter->second.push_back(tri);
}

void RopeNormaliser::appendTriangle(unsigned int a, unsigned int b, unsigned int c)
{
	if (a == b || a == c || b == c)
	{
		throw NMSLogicException("A triangle needs three disticnt lines");
	}
	mIndexBuffer.push_back(a);
	mIndexBuffer.push_back(b);
	mIndexBuffer.push_back(c);
	IndexTriangle tri(a, b, c, *mPoints);
	aggregateFaces(a, tri);
	aggregateFaces(b, tri);
	aggregateFaces(c, tri);
}

std::vector<std::pair<unsigned int, glm::vec3>> RopeNormaliser::createNormals()
{
	std::vector<std::pair<unsigned int, glm::vec3>> normals;
	for (auto& m : triangles)
	{
		unsigned int index = m.first;
		const std::vector<IndexTriangle>& tris = m.second;
		glm::vec3 sum = { 0,0,0 };
		for (const auto& v : tris)
		{
			sum += v.normal;
		}
		glm::vec3 n = glm::normalize(sum);
		normals.push_back({ index, n });
	}
	return normals;
}

