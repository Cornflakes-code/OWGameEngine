#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

#include "BoundingBox.h"
#include "Circle.h"
#include "VertexSource.h"

class TextBillboard;
/*
	Wraps the rendering of an XYZ set of labelled axis. Useful for early development
*/
class OWENGINE_API Axis
{
public:
	Axis();

	void prepare(const AABB& world);
	void render(const glm::mat4& proj, 
				const glm::mat4& view, 
				const glm::mat4& model) const;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	TextBillboard* mTextX;
	TextBillboard* mTextY;
	TextBillboard* mTextZ;
	VertexSource mZeroPoint;
	Circle mCircle;
	VertexSource mLines;
#pragma warning( pop )
};
