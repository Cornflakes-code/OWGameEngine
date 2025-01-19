#pragma once
#include <functional>
#include <string>
#include <algorithm>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "../Core/Renderable.h"
#include "../Renderers/RenderTypes.h"

class Shader;

class OWENGINE_API RendererBase
{
public:
	RendererBase(Shader* sh = nullptr): mShader(sh) {}

	const Shader* constShader() const  { return mShader; }
	virtual Shader* shader() { return mShader; }
	void shader(Shader* newShader) { mShader = newShader; }

	// OpenGL state functions
	void lineWidth(float width) { mLineWidth = width; }
	void polygonMode(unsigned int face, unsigned int mode) 
	{
		mPolygonFace = face; mPolygonMode = mode; 
	}
	void blendFunction(unsigned int sfactor, unsigned int dfactor)
	{
		mSfactor = sfactor;
		mDfactor = dfactor;
	}
	virtual void prepare() {}
	/*
		virtual void buildBoundingBox(AABB& bb, const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr);

	virtual void renderBoundingBox(const AABB& bb, const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr);
*/
	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr);
protected:
	virtual void doRender() const = 0;
	virtual void validateBase() const;
private:
	Shader* mShader;
#pragma warning( push )
#pragma warning( disable : 4251 )
	// OpenGL state variables. The default values are used as flags 
	// in the various RIAA Utility classes in CommonUtils.h

	float mLineWidth = -1.0f;

	unsigned int mPolygonFace = UINT_MAX;
	unsigned int mPolygonMode = UINT_MAX;

	unsigned int mSfactor = UINT_MAX;
	unsigned int mDfactor = UINT_MAX;

private:
#pragma warning( pop )
};