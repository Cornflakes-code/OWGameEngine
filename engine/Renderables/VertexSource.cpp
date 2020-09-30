#include "VertexSource.h"

void VertexSource::vertices(const std::vector<glm::vec3>& v,
	unsigned int location,
	unsigned int drawMode)
{
	mVec3 = v;
	mVertexLoc = location;
	mVertexMode = drawMode;
	assert(!mVec4.size() && mVec3.size());
}

void VertexSource::vertices(const std::vector<glm::vec4>& v,
	unsigned int location,
	unsigned int drawMode)
{
	mVec4 = v;
	mVertexLoc = location;
	mVertexMode = drawMode;
	assert(!mVec3.size() && mVec4.size());
}
