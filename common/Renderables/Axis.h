#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../Renderables/Renderer.h"
#include "../Renderables/BoundingBox.h"

class TextBillboard;
class Points;
class Shader;
class Camera;

class Axis: public Renderer
{
public:
	Axis(Shader* _shader = nullptr);

	void setUp(const AABB& world, const Camera* camera);
	void render(const glm::mat4& proj, const glm::mat4& view, 
				const glm::mat4& model) const;
private:
	TextBillboard* mX;
	TextBillboard* mY;
	TextBillboard* mZ;
	Points* mPoints;
	std::vector<glm::vec3> mLines;
};