#include "OWSphere.h"

#define _USE_MATH_DEFINES
#include <math.h>

//std::vector<glm::vec3> pointsOnSphere( int numHoroSegments, int numVertSegments, float TAU);
//std::vector<glm::vec3> fibonacciSphere(int numPoints, float TAU);

void OWSphere::drawSphere(int lats, int longs) 
{
    // https://stackoverflow.com/questions/7687148/drawing-sphere-in-opengl-without-using-glusphere
    for (int i = 0; i <= lats; i++)
    {
        double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        // glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= longs; j++) 
        {
            double lng = 2 * M_PI * (double)(j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            mVertices.push_back({ x * zr0, y * zr0, z0 }); // normal
            mVertices.push_back({ mRadius * x * zr0, mRadius * y * zr0, mRadius * z0 }); // vertex
            mVertices.push_back({ x * zr1, y * zr1, z1 }); // normal
            mVertices.push_back({ mRadius * x * zr1, mRadius * y * zr1, mRadius * z1 }); // vertex
        }
    }
}

bool OWSphere::intersects(const AABB& box, glm::vec3& normal, float& distance) const
{
	return false;
}

std::vector<glm::vec3> OWSphere::vertices()
{
	if (!mVertices.size())
	{
        drawSphere(20, 20);
	}

	return mVertices;
}
