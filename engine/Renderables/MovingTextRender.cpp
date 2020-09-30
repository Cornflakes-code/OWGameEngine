#include "MovingTextRender.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Helpers/TextBillboard.h"
#include "../Helpers/Shader.h"
#include "../Helpers/MovingText.h"

#include "../Renderables/Pyramid.h"

void MovingTextRender::addSource(MovingText& source)
{
	mPyramidRender.addSource(source.mPyramid);
	mTextRender.addSource(source.mText);
	source.mText = nullptr;
}

void MovingTextRender::render(const glm::mat4& proj, 
							const glm::mat4& view, 
							const glm::mat4& model,
							const MovingText* mt) const
{
	glm::mat4 temp = glm::translate(model, glm::vec3(mt->mBounds.minPoint().x,
		mt->mBounds.minPoint().y,
		mt->mBounds.minPoint().z));
	mTextRender.render(proj, view, temp);
	mPyramidRender.render(proj, view, model);
}