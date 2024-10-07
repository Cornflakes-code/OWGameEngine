#pragma once

#include <Renderers/RendererBase.h>

class MoveController;
class Shader;

class OWENGINE_API RendererBaseShader : public RendererBase
{
public:
	RendererBaseShader(Shader* shader);

	const Shader* shader() const override final { return mShader; }
protected:
	Shader* shader() override final { return mShader; }

	void validateBase() const override;

private:
#pragma warning( push )
#pragma warning( disable : 4251 )

private:
	Shader* mShader;
#pragma warning( pop )
};