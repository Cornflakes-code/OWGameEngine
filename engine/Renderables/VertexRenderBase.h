#pragma once

#include "../Helpers/CommonUtils.h"

class Vertices;

class OWENGINE_API VertexRenderBase
{
public:
	VertexRenderBase();
protected:
	void callResizeCallback(const Vertices* source,
		OWUtils::ResizeCallbackType resizeCb) const;
	void callRenderCallback(const Vertices* source,
		const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		OWUtils::RenderCallbackType renderCb) const;
	void checkBaseSourceForErrors(const Vertices* source);
	void renderPVM(const Vertices* source,
		const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model) const;
private:
	glm::vec2 scaleByAspectRatio(const glm::vec2& toScale) const;
#pragma warning( push )
#pragma warning( disable : 4251 )
	// Ok to modify Renderables if only for efficiency reasons
	mutable bool mFirstTimeRender = true;
	float aspectRatio() const;
#pragma warning( pop )
};