#include "TextRendererStatic.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Core/ErrorHandling.h"

#include "../Helpers/MeshDataLight.h"
#include "../Helpers/Shader.h"

TextRendererStatic::TextRendererStatic()
	: TextRenderer(new Shader("textStaticBillboard.v.glsl", "text.f.glsl", ""), 
				"VP")
{}

void TextRendererStatic::doSetup(const TextData* td, const glm::vec3& initialPosition)
{
	glm::vec3 position = glm::vec3(mBounds.center()) + initialPosition;
	shader()->appendMutator([position](const glm::mat4& proj, const glm::mat4& view,
		const glm::mat4& model, const glm::vec3& cameraPos, const Shader* shader)
	{
		glm::mat4 newModel =
			glm::translate(model, position);
		shader->setVector3f("BillboardPos", newModel[3]);
	});

	shader()->appendResizer([](const Shader* shader,
						RenderTypes::ScaleByAspectRatioType scaler,
						float aspectRatio)
	{
		glm::vec2 vv = { 0.5, 0.5 };
		glm::vec2 v2 = scaler(vv);
		shader->setVector2f("BillboardSize", v2);
	});
}
