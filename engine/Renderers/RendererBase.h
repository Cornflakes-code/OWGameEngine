#pragma once
#include <functional>
#include <string>
#include <algorithm>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

class MoveController;
class Shader;

class OWENGINE_API RendererBase
{
public:
	typedef std::function<void(const glm::mat4& proj, const glm::mat4& view,
					const glm::mat4& model, const glm::vec3& cameraPos,
					const Shader* shader)> RenderCallbackType;

	typedef std::function<glm::vec2(const glm::vec2)> ScaleByAspectRatioType;

	typedef std::function<void(const Shader* shader,
					ScaleByAspectRatioType scaler,
					float aspectRatio)> ResizeCallbackType;

	RendererBase(Shader* shader)
		:mShader(shader) {}
	
	// OpenGL state functions
	void appendRenderCallback(RenderCallbackType pfunc) { mRenderCallbacks.push_back(pfunc); }
	void appendResizeCallback(ResizeCallbackType pfunc) { mResizeCallbacks.push_back(pfunc); }
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
	virtual void prepare()
	{}

	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		MoveController* mover = nullptr,
		RenderCallbackType renderCb = nullptr,
		ResizeCallbackType resizeCb = nullptr) const;
	const Shader* shader() const { return mShader; }
protected:
	Shader* shader() { return mShader; }
	virtual void doRender() const = 0;
	void validateBase() const;
private:
	glm::vec2 scaleByAspectRatio(const glm::vec2& toScale) const;
	void callResizeCallback(ResizeCallbackType resizeCb) const;
	void callRenderCallback(const glm::mat4& proj, const glm::mat4& view,
							const glm::mat4& model, const glm::vec3& cameraPos,
							RenderCallbackType renderCb) const;
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
	std::vector<RenderCallbackType> mRenderCallbacks;
	std::vector<ResizeCallbackType> mResizeCallbacks;
	Shader* mShader;
	// After scene::setup it is Ok to modify Renderers
	// but only for efficiency reasons. Modifications
	// cannot change what is happenning else the game 
	// loop will be broken.
	mutable bool mFirstTimeRender = true;
	float aspectRatio() const;
#pragma warning( pop )
};