#include "RendererBaseShader.h"

#include "../Core/ErrorHandling.h"
#include "../Core/GlobalSettings.h"

#include "../Helpers/Shader.h"

RendererBaseShader::RendererBaseShader(Shader* shader)
	: RendererBase(), mShader(shader) {}

void RendererBaseShader::validateBase() const
{
	RendererBase::validateBase();
	if (shader() == nullptr)
		throw NMSLogicException("RendererBase::Shader must be set");
}

