#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "RenderTypes.h"

class Shader;
class OWENGINE_API OWIRenderable
{
	bool mRenderThis = true;
protected:
	bool renderThis() const { return mRenderThis;  }
public:
	void renderThis(bool newValue) { mRenderThis = newValue; }
	virtual void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr) = 0;

private:

};
