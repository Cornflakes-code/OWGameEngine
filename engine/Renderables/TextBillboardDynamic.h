#pragma once

#include "../OWEngine/OWEngine.h"

#include "../Helpers/TextBillboard.h"

/*
	Resizable Billboard Text
*/

class OWENGINE_API TextBillboardDynamic : public TextBillboard
{
public:
	TextBillboardDynamic(const glm::vec3& initialPosition,
						 const std::string& fontFileName, int fontHeight);
protected:
	void renderCallback(const glm::mat4& proj, const glm::mat4& view,
		const glm::mat4& model, Shader* shader);
	void resizeCallback(Shader* shader, 
		OWUtils::ScaleByAspectRatioType scaleByAspectRatio,
		float aspectRatio);
};