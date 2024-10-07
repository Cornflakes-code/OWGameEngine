#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <Renderers/RendererBase.h>
#include <Helpers/TextData.h>
#include "Shader.h"

struct ButtonData
{
	enum ClickState
	{
		MouseInside,
		MouseClicked
	};

	std::vector<glm::vec3> mButtonShape;
	TextData mText;
	bool intersectArea(const glm::vec3& pt);
	ClickState mClickState;
};

class VAOBuffer;
class TextRendererStatic;

// move to OWEngine once stable
class OWButton : public RendererBase
{
public:
	OWButton() : RendererBase() {}
	void setup(const ButtonData& data, const glm::vec3& position);
	void textures();
	void text();
	glm::vec3 position();
	void prepare() override;
	const Shader* shader() const override final;
protected:
	Shader* shader() override final;
	void doRender() const override;
private:
	ButtonData mData;
	VAOBuffer* mBuffer = nullptr;
	Shader* mShader;
	TextRendererStatic* mText;
};