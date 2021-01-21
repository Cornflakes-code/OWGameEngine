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
	typedef std::function<void(glm::mat4& proj, glm::mat4& view,
					glm::mat4& model, const Shader* shader)> RenderCallbackType;

	typedef std::function<glm::vec2(const glm::vec2)> ScaleByAspectRatioType;

	typedef std::function<void(const Shader* shader,
					ScaleByAspectRatioType scaler,
					float aspectRatio)> ResizeCallbackType;

	RendererBase(Shader* shader, const std::string& pvm)
		:mShader(shader), mPvm(pvm) {}
	
	void appendRenderCallback(RenderCallbackType pfunc) { mRenderCallbacks.push_back(pfunc); }
	void appendResizeCallback(ResizeCallbackType pfunc) { mResizeCallbacks.push_back(pfunc); }

	virtual void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const MoveController* mover = nullptr,
		RenderCallbackType renderCb = nullptr,
		ResizeCallbackType resizeCb = nullptr) const = 0;
protected:
	void setPVM(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model) const;
	const Shader* shader() const { return mShader; }
	glm::vec2 scaleByAspectRatio(const glm::vec2& toScale) const;
	void validateBase() const;
	void callResizeCallback(ResizeCallbackType resizeCb) const;
	void callRenderCallback(glm::mat4& proj, glm::mat4& view,
		glm::mat4& model, RenderCallbackType renderCb) const;
#pragma warning( push )
#pragma warning( disable : 4251 )

private:
	std::vector<RenderCallbackType> mRenderCallbacks;
	std::vector<ResizeCallbackType> mResizeCallbacks;
	Shader* mShader;
	std::string mPvm;
	// After scene::setup it is Ok to modify Renderers
	// but only for efficiency reasons. Modifications
	// cannot change what is happenning else the game 
	// loop will be broken.
	mutable bool mFirstTimeRender = true;
	float aspectRatio() const;
#pragma warning( pop )
};