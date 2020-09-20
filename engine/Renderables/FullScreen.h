#pragma once

#include <glm/glm.hpp>

#include "../Renderables/Renderer.h"
#include "../Helpers/ResizeHelper.h"

class Shader;
/*
	Temp class for investigating full screen sahders. Unlikely to survive
	future development
*/
class OWENGINE_API FullScreen: public Renderer, public ResizeHelper
{
#pragma warning( push )
#pragma warning( disable : 4251 )
#pragma warning( pop )
public:
	FullScreen(Shader* _shader = nullptr);
	void setUp(const AABB& world);
	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model) const override;
};