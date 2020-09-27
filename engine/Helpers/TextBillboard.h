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
#include "../Renderables/BoundingBox.h"

#include "../Renderables/VertexSource.h"
#include "ResourceFactory.h"
#include "ResizeHelper.h"

class Shader;

/*
	Base class for fixed and dynamic Billboard Text. Font texture loading is cached
	by FreeTypeFontAtlas via ResourceManager. Rendering is done in SimpleVCertexRender
*/
class OWENGINE_API TextBillboard: public VertexSource
{
protected:
	TextBillboard(const std::string& fontFileName, int fontHeight);
public:
	~TextBillboard();
	void createText(const std::string& text, float sx, float sy);
	void scale(const glm::vec2& newValue) { mScale = newValue; }
	const AABB& bounds() const { return mBounds; }


protected:
#pragma warning( push )
#pragma warning( disable : 4251 )
	const FreeTypeFontAtlas::FontDetails* mFontData;
	AABB mBounds;
	glm::vec2 mScale = { 0.5f, 0.5f };
#pragma warning( pop )
	AABB findBounds() const;
private:
};
