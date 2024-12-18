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
	throw NMSException("Incomplete function");
		Shader* sh = new Shader("button.v.glsl", "button.f.glsl", "");
	sh->setStandardUniformNames("pvm");
	VAOBuffer* vao = new VAOBuffer(sh, VAOBuffer::DRAW_ARRAYS);
	MeshDataLight lineData;
	lineData.vertices(data.mButtonShape, GL_TRIANGLES);
	lineData.polygonMode(GL_FILL);
	vao->add(&lineData);
	TextData* td = new TextData(new Physical(glm::vec3(10)), TextData::Static);
	//td->set(&(data.mText), glm::vec3(10));
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

void OWButton::textures()
{}

void OWButton::text()
{}


