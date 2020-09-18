#pragma once

#include "../OWEngine/OWEngine.h"

#include "../Helpers/TextBillboard.h"

/*
	Resizable Billboard Text
*/

class OWENGINE_API TextBillboardDynamic : public TextBillboard
{
public:
	TextBillboardDynamic(const std::string& fontFileName, int fontHeight);
protected:
	void doRender(const glm::mat4& proj, 
				  const glm::mat4& view, 
				  const glm::mat4& model) const override;
};