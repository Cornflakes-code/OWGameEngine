#pragma once

#include "../OWEngine/OWEngine.h"

#include "../Helpers/TextBillboard.h"
/*
	Fixed sized Billboard Text
*/
class OWENGINE_API TextBillboardFixed: public TextBillboard
{
public:
	TextBillboardFixed(const glm::vec3& initialPosition,
						const std::string& fontFileName, int fontHeight);
protected:
	void renderCallback(const glm::mat4& proj, const glm::mat4& view,
			const glm::mat4& model, const Shader* shader);
	void resizeCallback(const Shader* shader,
			OWUtils::ScaleByAspectRatioType scaleByAspectRatio,
			float aspectRatio);
};