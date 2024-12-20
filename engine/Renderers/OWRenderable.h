#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "RenderTypes.h"
#include "../Renderers/RenderTypes.h"

class Shader;
class OWENGINE_API OWRenderable
{
protected:
public:
public:
	virtual void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr) const = 0;

private:

};
