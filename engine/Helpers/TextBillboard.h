#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../Core/GLApplication.h"
#include "../Core/ListenerHelper.h"

#include "../Helpers/CommonUtils.h"
#include "../Helpers/BoundingBox.h"
#include "../Helpers/MoveTarget.h"

#include "../Renderables/SimpleModel.h"
#include "FreeTypeFontAtlas.h"

class Shader;

/*
	Base class for fixed and dynamic Billboard Text. Font texture loading is cached
	by FreeTypeFontAtlas via ResourceManager. Rendering is done in SimpleVCertexRender
*/
class OWENGINE_API TextBillboard: public SimpleModel
{
protected:
	TextBillboard(const glm::vec3& initialPosition, 
				  const std::string& fontFileName, int fontHeight);
public:
	~TextBillboard();
	void createText(const std::string& text, float sx, float sy);
	void scale(const glm::vec2& newValue) { mScale = newValue; }
protected:
#pragma warning( push )
#pragma warning( disable : 4251 )
	const FreeTypeFontAtlas::FontDetails* mFontData;
	glm::vec2 mScale = { 0.5f, 0.5f };
#pragma warning( pop )
	AABB findBounds() const;
private:
};
