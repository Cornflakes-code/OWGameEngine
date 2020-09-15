#pragma once

#include <vector>
#include <functional>

#include <glm/glm.hpp>

class Shader;
/*
	A work in progress. May be removed in later iterations
*/
class Renderer
{
public:
	virtual void render(const glm::mat4& proj,	
						const glm::mat4& view, 
						const glm::mat4& model) const = 0;
protected:
	Renderer(Shader* _shader);
	void prepareOpenGL(const std::vector<glm::vec3>& vertices,
		const std::vector<unsigned int>& indices,
		const glm::vec4& colour = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
	void renderOpenGL(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		unsigned int type,
		std::function<void()> cb = nullptr) const;
	Shader* mShader;
private:
	unsigned int mVao = 0;
	unsigned int mVbo = 0;
	unsigned int mEbo = 0;
	size_t mIndicesSize = 0;
};
