#include "TextComponent.h"

#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/GlobalSettings.h"
#include "../Core/ErrorHandling.h"
#include "../Helpers/Shader.h"
#include "../Helpers/FontFactory.h"
#include "../Renderers/MeshRenderer.h"
#include "../Helpers/Transform.h"

OWTextComponent::OWTextComponent(OWActor* _owner, const std::string& _name,
	const OWTextComponentData& _data)
	: OWMeshComponentBase(_owner, _name), mData(_data)
{
}

OWTextComponent::OWTextComponent(OWActor* _owner, const std::string& _name,
	const std::string& textFileName)
	: OWMeshComponentBase(_owner, _name)
{
	// Load mData from textFileName;
}

AABB adjustPosition(std::vector<glm::vec4>& v4, unsigned int mReferencePos)
{
	AABB bounds(v4);

	glm::vec4 displacement = glm::vec4(0);
	if (mReferencePos & OWTextComponentData::PositionType::Left)
	{
		displacement.x = -bounds.minPoint().x;
	}
	else if (mReferencePos & OWTextComponentData::PositionType::Right)
	{
		displacement.x = -bounds.maxPoint().x;
	}
	else
	{
		displacement.x = -bounds.center().x;
	}
	if (mReferencePos & OWTextComponentData::PositionType::Bottom)
	{
		displacement.y = -bounds.minPoint().y;
	}
	else if (mReferencePos & OWTextComponentData::PositionType::Right)
	{
		displacement.y = -bounds.maxPoint().y;
	}
	else
	{
		displacement.y = -bounds.center().y;
	}
	for (auto& v : v4)
	{
		v += displacement;
	}
	bounds.move(displacement);
	return bounds;
}

OWRenderTypes::ActorSetupMutator OWTextComponent::actorMutator(OWTextComponentData::TextDisplayType displayType)
{
	if (displayType == OWTextComponentData::TextDisplayType::Dynamic)
	{
		return 
		[](const OWCollider* coll, const OWMeshComponentBase* mesh,
			const OWPhysics* phys, OWTransform* trans, OWRenderer* rend)
			{
				if (!rend->mSSBO.locked(GPUBufferObject::BillboardSize))
				{
					const glm::vec2& magic = { 5.4f, 3.6f };
					glm::vec2 bbSize({ magic.x * glm::length(glm::vec3(trans->modelMatrix()[0])),
						magic.y * glm::length(glm::vec3(trans->modelMatrix()[1])) });
					rend->mSSBO.append(bbSize, GPUBufferObject::BillboardSize);
				}
			};
	}
	else if (displayType == OWTextComponentData::TextDisplayType::Static)
	{
		return
		[](const OWCollider* coll, const OWMeshComponentBase* mesh,
			const OWPhysics* phys, OWTransform* trans, OWRenderer* rend)
			{
				if (!rend->mSSBO.locked(GPUBufferObject::BillboardSize))
				{
					glm::vec2 bbSize = { glm::length(glm::vec3(trans->modelMatrix()[0])),
									glm::length(glm::vec3(trans->modelMatrix()[1]))};
					// This seems to work best (trial and error) when resizing the window.
					float _aspectRatio = globals->physicalWindowSize().x /
						(globals->physicalWindowSize().y * 1.0f);
					if (_aspectRatio < 1)
					{
						bbSize.x /= _aspectRatio;
						bbSize.y *= _aspectRatio;
					}
					else
					{
						bbSize.x /= _aspectRatio;
						//retval.y *= _aspectRatio ;
					}
					rend->mSSBO.append(bbSize, GPUBufferObject::BillboardSize);
				}
			};
	}
	else
	{
		throw NMSLogicException("OWTextComponent::shaderMutator. Unknown RenderType");
	}
}

OWRenderTypes::ShaderMutator OWTextComponent::shaderMutator(OWTextComponentData::TextDisplayType displayType)
{
	if (displayType == OWTextComponentData::TextDisplayType::Dynamic)
	{
		return
			[](const glm::mat4& proj, const glm::mat4& view,
				const glm::vec3& cameraPos, const Shader* shader)
			{
				glm::vec3 CameraRight_worldspace = { view[0][0], view[1][0], view[2][0] };
				shader->setVector3f("CameraRight_worldspace", CameraRight_worldspace);
				glm::vec3 CameraUp_worldspace = { view[0][1], view[1][1], view[2][1] };
				shader->setVector3f("CameraUp_worldspace", CameraUp_worldspace);
			};
	}
	else if (displayType == OWTextComponentData::TextDisplayType::Static)
	{
		return nullptr;
	}
	else
	{
		throw NMSLogicException("OWTextComponent::shaderMutator. Unknown RenderType");
	}
}

const OWRenderData OWTextComponent::renderData(AABB& bounds) const
{
	// Some good links to doing text better/faster
	// https://www.reddit.com/r/opengl/comments/ga2o7a/so_im_new_to_opengl_but_is_it_just_me_or_does/
	const FreeTypeFontAtlas::FontDetails& fontData
		= FontFactory().loadFreeTypeFont(mData.fontName, mData.fontHeight);
	MeshData md;
	md.v4 = fontData.createText(mData.text, mData.fontSpacing.x, mData.fontSpacing.y);
	if (md.v4.empty())
	{
		throw NMSLogicException(std::stringstream()
			<< "No Triangles generated for Text ["
			<< mData.text << "] is empty\n");
	}
	bounds = adjustPosition(md.v4, mData.referencePos);
	md.setColour(mData.colour, "textcolor");
	md.indicesMode = md.vertexMode = GL_TRIANGLES;
	md.setPolygonMode(GL_FILL);
	validate(md);
	OWRenderData retval;
	retval.meshes.push_back(md);
	FreeTypeFontAtlas::FontDetails fd
		= FontFactory().loadFreeTypeFont(mData.fontName, mData.fontHeight);
	retval.textures.push_back(fd.texture());
	return retval;
}

void OWTextComponent::doSetup()
{
	// If both left and right then remove them
	if (((mData.referencePos & OWTextComponentData::PositionType::Left) &&
		(mData.referencePos & OWTextComponentData::PositionType::Right)))
	{
		mData.referencePos = OWTextComponentData::PositionType(mData.referencePos & 0xC);
	}

	// If both Top and Bottom then remove them
	if (((mData.referencePos & OWTextComponentData::PositionType::Top) &&
		(mData.referencePos & OWTextComponentData::PositionType::Bottom)))
	{
		mData.referencePos = OWTextComponentData::PositionType(mData.referencePos & 0x3);
	}
}

AABB calcOrthogonalAABB(const glm::vec3& right, const glm::vec3& up,
	const glm::vec3& inOut, const AABB& orig, float distanceToEye)
{
	if (OWUtils::isZero(distanceToEye))
		distanceToEye = 0.001f;
	float dist = 20000 / distanceToEye;
	std::vector<glm::vec3> v;
	glm::vec3 sz = orig.size() * 0.5f; // mult by half extent.
	v.push_back(sz * right); //rhs
	v.push_back(-sz * right); // lhs
	v.push_back(sz * up); // up
	v.push_back(-sz * up); // down
	v.push_back(-sz * inOut); // in
	v.push_back(sz * inOut); // out
	for (glm::vec3& a : v)
	{
		a.x *= dist;
		a.y *= dist;
	}
	AABB bb(v);
	return bb;
}
