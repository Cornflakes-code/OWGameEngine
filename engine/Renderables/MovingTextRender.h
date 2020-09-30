#pragma once

#include "../OWEngine/OWEngine.h"

#include "BoundingBox.h"
#include "../Renderables/VertexRenderer.h"


/*
	A development class wrapping very simple collision functionality. Will be removed
	in later iterations
*/
class MovingText;

class OWENGINE_API MovingTextRender
{
public:
	void addSource(MovingText& source);
	void render(const glm::mat4& proj, 
				const glm::mat4& view, 
				const glm::mat4& model,
				const MovingText* mt) const;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	VertexRenderer mPyramidRender;
	VertexRenderer mTextRender;
#pragma warning( pop )
};