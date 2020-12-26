#include "VerticesRenderer.h"

#include "../Core/GlobalSettings.h"
#include "../Helpers/Shader.h"
#include "../Helpers/ErrorHandling.h"

#include "Vertices.h"

VerticesRenderer::VerticesRenderer()
{}

void VerticesRenderer::checkBaseSourceForErrors(const Vertices* source)
{
	checkRenderBaseForErrors(source);
	if (source->mVec3.size() == 0 && source->mVec4.size() == 0)
		throw NMSLogicException("both mVec3 and mVec4 are empty");
	if (source->mVec3.size() != 0 && source->mVec4.size() != 0)
		throw NMSLogicException("both mVec3 and mVec4 are not empty");
	if (source->mVertexLoc == GL_INVALID_INDEX)
		throw NMSLogicException("mVertexLoc is unset");
	if (source->mVertexMode == GL_INVALID_ENUM)
		throw NMSLogicException("mVertexMode is unset");
}

void VerticesRenderer::setPVM(const Vertices* source,
					const glm::mat4& proj,
					const glm::mat4& view,
					const glm::mat4& model) const
{
	if (!source->mPVMName.empty())
	{
		// Testing for name.size() is seriously tacky.
		// Must be a better way !!
		if (source->mPVMName.size() < 3)
		{
			glm::mat4 pvm = proj * view;
			source->mShader->setMatrix4(source->mPVMName, pvm);
		}
		else
		{
			glm::mat4 pvm = proj * view * model;
			source->mShader->setMatrix4(source->mPVMName, pvm);
		}
	}
}
