#include "RopeNormaliser.h"

#include <Core/ErrorHandling.h>

unsigned int RopeNormaliser::append(std::vector<glm::vec3>& v, const glm::vec3& p)
{
	v.emplace_back(p);
	v.emplace_back(glm::vec3(0.0f));// placeholder for normal values
	return static_cast<unsigned int>(v.size()) - 2;
}

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
		throw NMSLogicException("A triangle needs three distinct lines");
	}
	mIndexBuffer.push_back(a);
	mIndexBuffer.push_back(b);
	mIndexBuffer.push_back(c);
	IndexTriangle tri(a, b, c, *mPoints);
	aggregateFaces(a, tri);
	aggregateFaces(b, tri);
	aggregateFaces(c, tri);
}

void RopeNormaliser::createNormals(std::vector<glm::vec3>& points, unsigned int offsetFromVertex, unsigned int stride)
{
	bool smoothNormals = true;
	if (smoothNormals) // ie face normals
	{
		// https://computergraphics.stackexchange.com/questions/4031/programmatically-generating-vertex-normals
		// https://iquilezles.org/articles/normals/
		for (auto& t : triangles)
		{
			const std::vector<IndexTriangle>& tris = t.second;
			for (const auto& v : tris)
			{
				glm::vec3 edge1 = (*mPoints)[v.indicies[0]] - (*mPoints)[v.indicies[1]];
				glm::vec3 edge2 = (*mPoints)[v.indicies[2]] - (*mPoints)[v.indicies[1]];
				glm::vec3 p = glm::cross(edge1, edge2);
				points[v.indicies[0] + offsetFromVertex] += p;
				points[v.indicies[1] + offsetFromVertex] += p;
				points[v.indicies[2] + offsetFromVertex] += p;
			}
		}
		for (int i = offsetFromVertex; i < points.size(); i += stride)
		{
			points[i] = -glm::normalize(points[i]);
		}
	}
	else // flat ie vertex normals
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
		for (const auto& each : normals)
		{
			unsigned int ndx = each.first;
			glm::vec3 normal = each.second;
			points[ndx + offsetFromVertex] = normal;
		}
	}
}

