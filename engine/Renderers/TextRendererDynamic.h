#pragma once
#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "TextRenderer.h"

class TextData;

class OWENGINE_API TextRendererDynamic: public TextRenderer
{
public:
	TextRendererDynamic();
protected:
	void doSetup(const TextData* td, const glm::vec3& initialPosition) override;
private:
};