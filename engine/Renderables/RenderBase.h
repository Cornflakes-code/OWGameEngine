#pragma once

#include "../Helpers/CommonUtils.h"

class RenderableBase;

class OWENGINE_API RenderBase
{
public:
	RenderBase();
protected:
	void checkRenderBaseForErrors(const RenderableBase* source) const;
	glm::vec2 scaleByAspectRatio(const glm::vec2& toScale) const;
	void callResizeCallback(const RenderableBase* source,
		OWUtils::ResizeCallbackType resizeCb) const;
	void callRenderCallback(const RenderableBase* source,
		const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		OWUtils::RenderCallbackType renderCb) const;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	// Ok to modify Renderables if only for efficiency reasons
	mutable bool mFirstTimeRender = true;
	float aspectRatio() const;
#pragma warning( pop )
};