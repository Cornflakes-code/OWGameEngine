#include "Button.h"

#include <glm/gtx/transform.hpp>

#include "../Helpers/Shader.h"
#include "../Renderers/VAOBuffer.h"
#include "../Renderers/TextRendererStatic.h"
#include "../Component/TextComponent.h"

OWButton::OWButton(Scene* _owner, ButtonScript* _data)
	:OWActor(_owner, _data)
{ }

bool ButtonData::intersectArea(const glm::vec3& pt)
{
	return false;
}

void OWButton::setup(const ButtonData& data, const glm::vec3& position)
{
	throw NMSException("Incomplete function");
	ShaderData* shd = new ShaderData("button.v.glsl", "button.f.glsl", "", "pvm");
	if (false)
	{
		auto mut = [position](const glm::mat4& proj, const glm::mat4& view,
			const glm::mat4& model, const glm::vec3& cameraPos,
			const Shader* shader)
			{
				glm::vec3 CameraRight_worldspace = { view[0][0], view[1][0], view[2][0] };
				shader->setVector3f("CameraRight_worldspace", CameraRight_worldspace);
				glm::vec3 CameraUp_worldspace = { view[0][1], view[1][1], view[2][1] };
				shader->setVector3f("CameraUp_worldspace", CameraUp_worldspace);
				glm::mat4 newModel = glm::translate(model, position);
				shader->setVector3f("BillboardPos", newModel[3]);
			};
		shd->mutatorCallbacks.push_back(mut);
	}
	Shader* sh = new Shader(shd);
	VAOBuffer* vao = new VAOBuffer(sh, VAOBuffer::DRAW_ARRAYS);
	MeshDataLight lineData;
	lineData.vertices(data.mButtonShape, GL_TRIANGLES);
	lineData.polygonMode(GL_FILL);
	vao->add(&lineData);
	TextComponentData* tdc = new TextComponentData();
	tdc->textData.tdt = TextData::TextDisplayType::Static;
	tdc->physics.position = glm::vec3(10);
	TextComponent* td = new TextComponent(this, tdc);
}

void OWButton::textures()
{}

void OWButton::text()
{}


