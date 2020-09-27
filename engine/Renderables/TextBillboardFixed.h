#pragma once

#include "../OWEngine/OWEngine.h"

#include "../Helpers/TextBillboard.h"
/*
	Fixed sized Billboard Text
*/
class OWENGINE_API TextBillboardFixed: public TextBillboard
{
public:
	TextBillboardFixed(const std::string& fontFileName, int fontHeight);
protected:
	void renderCallback(const glm::mat4& proj, const glm::mat4& view,
			const glm::mat4& model, Shader* shader);
	void resizeCallback(Shader* shader,
			ResizeHelper::ScaleByAspectRatioType scaleByAspectRatio,
			float aspectRatio);
};