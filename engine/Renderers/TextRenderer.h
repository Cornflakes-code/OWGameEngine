#pragma once
#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "../Core/BoundingBox.h"

#include "../Helpers/Texture.h"

#include "RendererBase.h"

struct TextData;

class OWENGINE_API TextRenderer: public RendererBase
{
public:
	TextRenderer(Shader* shader, const std::string& pvm)
		: RendererBase(shader, pvm) {}
	void setup(const TextData* td, 
				const glm::vec3& initialPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	AABB bounds() const { return mBounds; }
	virtual void doRender() const override;
protected:
	virtual void doSetup(const TextData* td, const glm::vec3& initialPosition) = 0;
#pragma warning( push )
#pragma warning( disable : 4251 )
	glm::vec4 mColour = glm::vec4();
	Texture mTexture;
	AABB mBounds;
	size_t mV4Size = 0;
	unsigned int mVao = 0;
	unsigned int mVbo = 0;
#pragma warning( pop )
private:
	void validate(const TextData* td) const;
};