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

	appendRenderCallback([position](glm::mat4& proj, glm::mat4& view,
		glm::mat4& model, const Shader* shader)
	{
		glm::mat4 newModel =
			glm::translate(model, position);
		shader->setVector3f("BillboardPos", newModel[3]);
	});

	appendResizeCallback([](const Shader* shader,
						ScaleByAspectRatioType scaler,
						float aspectRatio)
	{
		glm::vec2 vv = { 0.5, 0.5 };
		glm::vec2 v2 = scaler(vv);
		shader->setVector2f("BillboardSize", v2);
	});
}
