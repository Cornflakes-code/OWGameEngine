#pragma once

#include <functional>
#include <glm/glm.hpp>

class Shader;
class OWCollider;
class OWMeshComponentBase;
class OWPhysics;
class OWTransform;
class OWRenderer;

namespace OWRenderTypes
{
	typedef std::function<void(const glm::mat4& proj, const glm::mat4& view,
		const glm::vec3& cameraPos, const Shader* shader)> ShaderMutator;

	typedef std::function<glm::vec2(const glm::vec2)> ScaleByAspectRatioType;

	typedef std::function<void(const Shader* shader,
		ScaleByAspectRatioType scaler,
		float aspectRatio)> ShaderResizer;

	typedef std::function<void(const OWCollider* coll, const OWMeshComponentBase* mesh,
		const OWPhysics* phys, OWTransform* trans, OWRenderer* rend)> ActorSetupMutator;

};
