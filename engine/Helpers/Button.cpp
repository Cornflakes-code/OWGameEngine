#include "Button.h"

#include <glm/gtx/transform.hpp>

#include <Renderers/VAOBuffer.h>
#include <Helpers/Shader.h>
#include <Renderers/TextRendererStatic.h>

bool ButtonData::intersectArea(const glm::vec3& pt)
{
	return false;
}

void OWButton::setup(const ButtonData& data, const glm::vec3& position)
{
	mData = data;
	mShader = new Shader("button.v.glsl", "button.f.glsl", "");
	mShader->setStandardUniformNames("pvm");
	mBuffer = new VAOBuffer(mShader);
	MeshDataLight lineData;
	lineData.vertices(data.mButtonShape, GL_TRIANGLES);
	lineData.polygonMode(GL_FILL);
	mBuffer->add(&lineData);
	mText = new TextRendererStatic();
	mText->setup(&(mData.mText), glm::vec3(10));
	if (false)
	{
		appendRenderCallback([position](const glm::mat4& proj, const glm::mat4& view,
			const glm::mat4& model, const glm::vec3& cameraPos,
			const Shader* shader)
			{
				glm::vec3 CameraRight_worldspace = { view[0][0], view[1][0], view[2][0] };
				shader->setVector3f("CameraRight_worldspace", CameraRight_worldspace);
				glm::vec3 CameraUp_worldspace = { view[0][1], view[1][1], view[2][1] };
				shader->setVector3f("CameraUp_worldspace", CameraUp_worldspace);
				glm::mat4 newModel = glm::translate(model, position);
				shader->setVector3f("BillboardPos", newModel[3]);
			});
	}
}

const Shader* OWButton::shader() const
{
	return mShader;
}

Shader* OWButton::shader() 
{
	return mShader;
}

void OWButton::textures()
{}

void OWButton::text()
{}

glm::vec3 OWButton::position()
{
	return glm::vec3(0);
}

void OWButton::prepare()
{
}

void OWButton::doRender() const
{
	//mBuffer->doRender();
	mText->doRender();
}

