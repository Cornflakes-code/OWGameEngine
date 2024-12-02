#include "TextRendererDynamic.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Core/BoundingBox.h"
#include "../Core/ErrorHandling.h"

#include "../Helpers/MeshDataLight.h"
#include "../Helpers/Shader.h"
#include "../Helpers/TextData.h"

TextRendererDynamic::TextRendererDynamic()
	: TextRenderer(new Shader("textDynamicBillboard.v.glsl", "text.f.glsl", ""), 
					"VP")
{
	shader()->setStandardUniformNames("VP");
}

void TextRendererDynamic::doSetup(const TextData* td, const glm::vec3& initialPosition)
{
	glm::vec3 position = glm::vec3(mBounds.center()) + initialPosition;

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
	
	AABBV3 bounds = mBounds;
	glm::vec2 scale = td->scale();
	appendResizeCallback([bounds, scale](const Shader* shader,
			ScaleByAspectRatioType scaler,
			float aspectRatio)
	{
		glm::vec2 bbSize({ bounds.size().x * scale.x, bounds.size().y * scale.y });
		shader->setVector2f("BillboardSize", bbSize);
	});

}

