#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <Core/Actor.h>
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
class OWButton : public Actor
{
public:
	OWButton(Physical* _physical) : Actor(_physical, nullptr) {}
	void setup(const ButtonData& data, const glm::vec3& position);
	void textures();
	void text();
protected:
private:
};