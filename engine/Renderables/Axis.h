#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

#include "../Renderables/BoundingBox.h"
#include "../Renderables/VertexRenderer.h"
#include "../Renderables/VertexRendererCompound.h"
#include "../Renderables/VertexSourceCompound.h"

class TextBillboard;
class Points;
class Shader;
class Camera;
/*
	Wraps the rendering of an XYZ set of labelled axis. Useful for early development
*/
class OWENGINE_API Axis
{
public:
	Axis();

	void setUp(const AABB& world, const Camera* camera);
	void render(const glm::mat4& proj, 
				const glm::mat4& view, 
				const glm::mat4& model) const;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::vector<glm::vec3> mAxisCoords;
	VertexRenderer mZeroPoint;
	VertexRenderer mTextX;
	VertexRenderer mTextY;
	VertexRenderer mTextZ;
	VertexRenderer mCircle;
	VertexRenderer mLines;
#pragma warning( pop )
};
