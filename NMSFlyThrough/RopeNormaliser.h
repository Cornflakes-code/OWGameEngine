#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>


class RopeNormaliser
{
	struct IndexTriangle
	{
		IndexTriangle(unsigned int a, unsigned int b, unsigned int c, const std::vector<glm::vec3>& points);

		unsigned int indicies[3];
		glm::vec3 normal;
	};

	const std::vector<glm::vec3>* mPoints;
	typedef std::map<unsigned int, std::vector<IndexTriangle>> TriAngleElements;
	TriAngleElements triangles;
	void aggregateFaces(unsigned int ndx, const IndexTriangle& tri);
public:
	static std::vector<unsigned int> mIndexBuffer;
	RopeNormaliser(const std::vector<glm::vec3>* p)
		:mPoints(p)
	{}
	void appendTriangle(unsigned int a, unsigned int b, unsigned int c);
	void createNormals(std::vector<glm::vec3>& points, unsigned int offsetFromVertex, unsigned int stride);
};