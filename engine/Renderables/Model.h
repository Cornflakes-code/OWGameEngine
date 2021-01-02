#pragma once

#include <vector>
#include <functional>
#include <memory>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "../Helpers/ErrorHandling.h"
#include "RenderableBase.h"

struct ModelData;
class ModelRenderer;
/*
	Basically a struct that contains the data to be rendered.
	Passed to VertexRenderer to be rendered.
	Can be used as is or as a base class for more complex graphics.
	No getters. Values only accessible by VertexRenderer and derived classes.
	Does this means it cannot be inherited from outside of the DLL?
*/
class OWENGINE_API Model : public RenderableBase
{
public:
	Model(const glm::vec3& _initialPosition = glm::vec3(0.0f, 0.0f, 0.0f))
		: RenderableBase(_initialPosition) {}
	void create(const std::string& modelFileName, bool cache = true);
	void renderer(ModelRenderer* source);
	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const MoveController* mover = nullptr,
		OWUtils::RenderCallbackType renderCb = nullptr,
		OWUtils::ResizeCallbackType resizeCb = nullptr) const override;
protected:
	AABB calcBounds() const override;

#pragma warning( push )
#pragma warning( disable : 4251 )
	ModelRenderer* mRenderer = nullptr;
	ModelData* mRootNode;
#pragma warning( pop )
	friend class ModelFactory;
	friend class ModelRenderer;
};
