#pragma once
#include "../OWEngine/OWEngine.h"

#include <vector>
#include <functional>
#include <memory>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../Core/CommonUtils.h"
#include "../Core/ErrorHandling.h"

#include "../Helpers/ModelData.h"

#include "../Renderers/RendererBase.h"

struct ModelData;
class ModelRenderer;
class MoveController;
/*
	Basically a struct that contains the data to be rendered.
	Passed to VertexRenderer to be rendered.
	Can be used as is or as a base class for more complex graphics.
	No getters. Values only accessible by VertexRenderer and derived classes.
	Does this means it cannot be inherited from outside of the DLL?
*/
class OWENGINE_API Model
{
public:
	typedef std::function<glm::vec2(const glm::vec2)> ScaleByAspectRatioType;

	void append(ModelData* md);
	void setup(ModelData* md);
	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		MoveController* mover = nullptr,
		RendererBase::RenderCallbackType renderCb = nullptr,
		RendererBase::ResizeCallbackType resizeCb = nullptr) const;
private:
	ModelData mRootNode;
};