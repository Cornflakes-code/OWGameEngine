#pragma once

#include "../Helpers/TextBillboard.h"

/*
	Resizable Billboard Text
*/

class TextBillboardDynamic : public TextBillboard
{
public:
	TextBillboardDynamic(const std::string& fileName, int fontHeight);
protected:
	void doRender(const glm::mat4& proj, 
				  const glm::mat4& view, 
				  const glm::mat4& model) override;
};