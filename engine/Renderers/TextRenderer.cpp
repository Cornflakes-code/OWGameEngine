#include "TextRenderer.h"

#include "../Core/ErrorHandling.h"

#include "../Helpers/FontFactory.h"
#include "../Helpers/Shader.h"

void OWTextRenderer::doSetup(const std::vector<MeshData>& meshes,
	const std::vector<InstanceData>& instances,
	const std::vector<OWModelData>& models)
{
	throw NMSLogicException(
		"OWTextRenderer::doSetup not use for text.\n");

}

void OWTextRenderer::initText(const TextData& td, const glm::vec3& initialPosition)
{
	validate(td);
	const FreeTypeFontAtlas::FontDetails* fontData
		= FontFactory().loadFreeTypeFont(td->mFontFileName, td->mFontHeight);
	std::vector<glm::vec4> v4 = fontData->createText(td->mText, td->mX, td->mY);
	if (v4.empty())
	{
		throw NMSLogicException(std::stringstream()
			<< "No Triangles generated for Text ["
			<< td->mText << "] is empty\n");
	}
	mColour = td->mColour;
	mBounds = adjustPosition(v4, td->mReferencePos);

	// render::glDrawArrays needs the size of the buffer. This allows v4 to be cleared
	// if it is not going to be modified.
	mV4Size = v4.size();

	mTexture = fontData->texture();
	mTexture.samplerName("textureImageId");

	glGenVertexArrays(1, &mVao);
	// bind the Vertex Array Object first, then bind and set 
	// vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(mVao);

	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);

	shader()->use();
	shader()->setVector4f("textcolor", td->mColour);
	unsigned int vertexLoc = shader()->getAttributeLocation("coord");
	glVertexAttribPointer(vertexLoc,
		4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(vertexLoc);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * v4.size(),
		v4.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	// You can unbind the VAO afterwards so other VAO calls won't 
	// accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't 
	// unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	blendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	doSetup(td, initialPosition);
}

void TextRenderer::doRender() const
{
	constShader()->setVector4f("textcolor", mColour);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(mVao);
	glActiveTexture(mTexture.imageUnit());
	glBindTexture(mTexture.target(), mTexture.location());
	// associate sampler with name in shader
	//shader()->setInteger(mTexture.samplerName(), mTexture.imageUnit() - GL_TEXTURE0);

	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mV4Size));
	glBindVertexArray(0);
	glActiveTexture(mTexture.imageUnit());
	glBindTexture(mTexture.target(), 0);
}

void TextRenderer::validate(const TextData* td) const
{
	validateBase();
	if (td->mText.empty())
		throw NMSLogicException("Text to display is empty\n");
	if (td->mFontFileName.empty())
	{
		throw NMSLogicException(std::stringstream()
			<< "Font File name for Text ["
			<< td->mText << "] is empty\n");
	}
}
