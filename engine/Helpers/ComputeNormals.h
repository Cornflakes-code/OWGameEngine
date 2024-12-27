#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>

namespace GLMHelpers
{
    std::vector<glm::vec3> createNormals(const std::vector<glm::vec3>& tris);
}

struct MeshDataLight;

class ComputeNormals
{
	struct IndexTriangle
	{
		IndexTriangle(unsigned int a, unsigned int b, unsigned int c, const std::vector<glm::vec3>& points);

		unsigned int indicies[3];
		glm::vec3 normal;
	};

	const std::vector<glm::vec3> mPoints;
	typedef std::map<unsigned int, std::vector<IndexTriangle>> TriAngleElements;
	TriAngleElements triangles;
	void aggregateFaces(unsigned int ndx, const IndexTriangle& tri);
	std::vector<unsigned int> mIndexBuffer;
	static unsigned int append(std::vector<glm::vec3>& v, const glm::vec3& p);
	void appendTriangle(const std::vector<glm::vec3>& triAngles, unsigned int a, unsigned int b, unsigned int c);
	void createNormals(std::vector<glm::vec3>& triAngles, unsigned int offsetFromVertex, unsigned int stride);
public:
	ComputeNormals(const std::vector<glm::vec3>& p)
		:mPoints(p)
	{}

	// Call compute when mPoints contains vec3 already arranged in triangles
	// populates data with vertices and indices
	MeshDataLight compute();
};