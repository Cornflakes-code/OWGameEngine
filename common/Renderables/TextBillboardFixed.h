#pragma once

#include "../Helpers/TextBillboard.h"
/*
	Fixed sized Billboard Text
*/
class TextBillboardFixed: public TextBillboard
{
public:
	TextBillboardFixed(const std::string& fileName, int fontHeight);
protected:
	void doRender(const glm::mat4& proj, 
				  const glm::mat4& view,
				  const glm::mat4& model) override;
};