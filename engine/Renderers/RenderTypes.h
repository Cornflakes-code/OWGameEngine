#pragma once

#include <functional>
#include <glm/glm.hpp>

class Shader;

namespace RenderTypes
{
	typedef std::function<void(const glm::mat4& proj, const glm::mat4& view,
		const glm::mat4& model, const glm::vec3& cameraPos,
		const Shader* shader)> ShaderMutator;

	typedef std::function<glm::vec2(const glm::vec2)> ScaleByAspectRatioType;

	typedef std::function<void(const Shader* shader,
		ScaleByAspectRatioType scaler,
		float aspectRatio)> ShaderResizer;
};
