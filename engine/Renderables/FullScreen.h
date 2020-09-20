#pragma once

#include "../OWEngine/OWEngine.h"

#include "../Renderables/SimpleVertexSource.h"

class OWENGINE_API FullScreen:
	public SimpleVertexSource
{
public:
	FullScreen(Shader* _shader);
	GLuint texture() const override { return 0; }
	const Shader* shader() const override { return mShader; }
	std::vector<glm::vec4> vertices(std::string& verticeLocation,
		unsigned int& type) const override
	{
		type = GL_TRIANGLES;
		verticeLocation = "coord";
		return mVertices;
	}
	std::vector<unsigned int> indices(unsigned int& type) const override
	{
		type = 0;
		return std::vector<unsigned int>();
	}
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::vector<glm::vec4> mVertices;
	Shader* mShader;
#pragma warning( pop )
};