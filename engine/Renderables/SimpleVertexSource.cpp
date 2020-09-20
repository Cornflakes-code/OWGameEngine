#include "SimpleVertexSource.h"

#include "../Helpers/Shader.h"

SimpleVertexSource::SimpleVertexSource(const std::string& pvmName)
	:mPvmName(pvmName)
{}

void SimpleVertexSource::doRender(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model) const
{
	if (mPvmName.size())
	{
		shader()->use();
		glm::mat4 pvm = proj * view * model;
		shader()->setMatrix4(mPvmName, pvm);
	}
}
