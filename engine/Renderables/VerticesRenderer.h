#pragma once

#include "../Helpers/CommonUtils.h"
#include "RenderBase.h"

class Vertices;

class OWENGINE_API VerticesRenderer: public RenderBase
{
public:
	VerticesRenderer();
protected:
	void checkBaseSourceForErrors(const Vertices* source) const;
	void setPVM(const Vertices* source,
		const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model) const;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	// Ok to modify Renderables if only for efficiency reasons
	mutable bool mFirstTimeRender = true;
	float aspectRatio() const;
#pragma warning( pop )
};