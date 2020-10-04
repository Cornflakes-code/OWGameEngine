#pragma once

#include <glm/glm.hpp>
#include "../Renderables//BoundingBox.h"
#include "../Renderables/VertexSource.h"

class Shader;
/*
	Temp class for investigating full screen sahders. Unlikely to survive
	future development
*/
class OWENGINE_API FullScreen: public VertexSource
{
#pragma warning( push )
#pragma warning( disable : 4251 )
#pragma warning( pop )
public:
	FullScreen(const glm::vec3& initialPosition, 
				Shader* _shader, const std::string& pvm);
	void prepare(const AABB& world);
protected:
	void renderCallback(const glm::mat4& proj, const glm::mat4& view,
		const glm::mat4& model, Shader* shader);
	void resizeCallback(Shader* shader,
		OWUtils::ScaleByAspectRatioType scaleByAspectRatio,
		float aspectRatio);
};