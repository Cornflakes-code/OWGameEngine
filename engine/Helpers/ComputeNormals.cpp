#include "ComputeNormals.h"

#include <Core/ErrorHandling.h>
#include "../Helpers/MeshDataLight.h"

namespace GLMHelpers
{
	// This may hae some info
	// https://iquilezles.org/articles/normals/
    std::vector<glm::vec3> createNormals(const std::vector<glm::vec3>& tris)
    {
        std::vector<glm::vec3> vertPlusNormals;
        for (int i = 0; i < tris.size(); i += 3)
        {
            glm::vec3 edge1 = tris[i] - tris[i + 1];
            glm::vec3 edge2 = tris[i + 2] - tris[i + 1];
            glm::vec3 n = glm::cross(edge1, edge2);
            vertPlusNormals.push_back(tris[i]);
            vertPlusNormals.push_back(tris[i + 1]);
            vertPlusNormals.push_back(tris[i + 2]);
            vertPlusNormals.push_back(glm::normalize(n));
        }
        return vertPlusNormals;
    }
}

// I could also do it this way
// https://gamedev.stackexchange.com/questions/133864/calculating-vertex-normals-in-opengl-c?newreg=310c1117667f4ec99cd28866d3a49149

// And this may have some info
// https://community.khronos.org/t/surface-normal-function-not-really-sure/62685
unsigned int ComputeNormals::append(std::vector<glm::vec3>& v, const glm::vec3& p)
{
	v.emplace_back(p);
	v.emplace_back(glm::vec3(0.0f));// placeholder for normal values
	return static_cast<unsigned int>(v.size()) - 2;
}

ComputeNormals::IndexTriangle::IndexTriangle(unsigned int a, unsigned int b, unsigned int c, const std::vector<glm::vec3>& points)
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

void ComputeNormals::aggregateFaces(unsigned int ndx, const IndexTriangle& tri)
{
	TriAngleElements::iterator iter = triangles.find(ndx);
	if (iter == triangles.end())
		triangles.insert({ ndx, {tri} });
	else
		iter->second.push_back(tri);
}

void ComputeNormals::appendTriangle(const std::vector<glm::vec3>& triAngles, unsigned int a, unsigned int b, unsigned int c)
{
	if (a == b || a == c || b == c)
	{
		throw NMSLogicException("A triangle needs three distinct lines");
	}
	mIndexBuffer.push_back(a);
	mIndexBuffer.push_back(b);
	mIndexBuffer.push_back(c);
	IndexTriangle tri(a, b, c, triAngles);
	aggregateFaces(a, tri);
	aggregateFaces(b, tri);
	aggregateFaces(c, tri);
}

void ComputeNormals::createNormals(std::vector<glm::vec3>& triAngles, unsigned int offsetFromVertex, unsigned int stride)
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
				glm::vec3 edge1 = triAngles[v.indicies[0]] - triAngles[v.indicies[1]];
				glm::vec3 edge2 = triAngles[v.indicies[2]] - triAngles[v.indicies[1]];
				glm::vec3 p = glm::cross(edge1, edge2);
				triAngles[v.indicies[0] + offsetFromVertex] += p;
				triAngles[v.indicies[1] + offsetFromVertex] += p;
				triAngles[v.indicies[2] + offsetFromVertex] += p;
			}
		}
		for (int i = offsetFromVertex; i < triAngles.size(); i += stride)
		{
			triAngles[i] = -glm::normalize((triAngles)[i]);
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
			triAngles[ndx + offsetFromVertex] = normal;
		}
	}
}

MeshDataLight ComputeNormals::compute()
{
	MeshDataLight data;
	std::vector<unsigned int> tempWireIndices(mPoints.size());
	std::vector<glm::vec3> trianglePoints;
	for (int i = 0; i < mPoints.size(); i++)
	{
		tempWireIndices[i] = ComputeNormals::append(trianglePoints, mPoints[i]);
	}
	for (int i = 0; i < mPoints.size(); i += 3)
	{
		appendTriangle(trianglePoints, tempWireIndices[i], tempWireIndices[i + 1], tempWireIndices[i + 2]);
	}
	createNormals(trianglePoints, 1, 2);
	data.vertices(trianglePoints, GL_TRIANGLES);
	data.indices(mIndexBuffer, GL_TRIANGLES);
	return data;
}
