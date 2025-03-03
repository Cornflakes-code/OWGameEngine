#include "OWSceneComponent.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Geometry/OWRay.h"
#include "../Actor/OWActor.h"
#include "../Helpers/Shader.h"


OLDSceneComponent::OLDSceneComponent(OLDActor* _owner, OLDSceneComponentData* _data)
	: OLDComponent(_owner)
{
	_data->component = this;
	setData(_data);
	name(_data->name);
	_owner->addSceneComponent(this);
}

void OLDSceneComponent::doInit()
{
	if (mBoundingBoxRenderer == nullptr)
	{
		ShaderData* sd = new ShaderData();
		sd->PVMName = "pvm";
		MeshComponentVAOData* mvd = new MeshComponentVAOData();
		mvd->shaderData.shaderV = "Wires.v.glsl";
		mvd->shaderData.shaderF = "Wires.f.glsl";
		mvd->shaderData.PVMName = "pvm";
		mvd->shaderData.projectionName = "projection";
		mvd->shaderData.viewName = "view";
		mvd->shaderData.modelName = "model";

		Shader* lineShader = new Shader(&mvd->shaderData);
		lineShader->loadBoilerPlates();
		mvd->meshData = VAOBuffer(lineShader, VAOBuffer::DRAW_MULTI);

		VAOBuffer* vao = new VAOBuffer(lineShader, VAOBuffer::DRAW_ARRAYS);
		std::vector<std::vector<glm::vec3>> surfaces = data()->boundingBox.surfaces();
		for (const std::vector<glm::vec3>& p : surfaces)
		{
			MeshDataLight lineData;
			lineData.colour(OWUtils::colour(OWUtils::SolidColours::BLACK), "colour");
			lineData.vertices(p, GL_LINE_STRIP);
			vao->add(&lineData);
		}
		vao->prepare();
		mBoundingBoxRenderer = vao;
	}
	else
		throw NMSLogicException(
			"Error. mBoundingBoxRenderer should be null OWSceneComponent::doInit() for [" + name() + "]\n");
	mOriginalBoundingBox = data()->boundingBox;
	physicalDoInit();
}

bool OLDSceneComponent::canCollide()
{
	return true;
}

bool OLDSceneComponent::canCollide(OLDCollisionData* other) 
{
	if (other->component == this)
		return false;
	// called before testing for couldCollide
	return true;

}
bool OLDSceneComponent::collides(OLDCollisionData* other)
{
	return false;
}

void OLDSceneComponent::collided(OLDCollisionData* other) 
{
	// https://gamedev.stackexchange.com/questions/47888/find-the-contact-normal-of-rectangle-collision?noredirect=1&lq=1
	const glm::vec3 otherRelVel = other->component->data()->physics.velocity;
	glm::vec3 relVel = data()->physics.velocity - otherRelVel;
	glm::vec3 compoundSize(data()->boundingBox.size() + other->boundingBox.size());
	AABB compoundAABB(compoundSize);
	glm::vec3 jfw = data()->boundingBox.center();
	compoundAABB.moveTo(data()->boundingBox.center());
	glm::vec3 normal1, normal2;
	float distance1, distance2;

	OWRay r1(data()->boundingBox.center(), relVel);
	if (!r1.intersects(data()->boundingBox, normal1, distance1))
	{
		return;
	}
	OWRay r31(data()->boundingBox.center(), relVel);
	if (!r1.intersects(compoundAABB, normal2, distance2))
	{
		return;
	}
	// Various collision detection algorythmns
	// https://developer.nvidia.com/gpugems/gpugems3/part-v-physics-simulation/chapter-32-broad-phase-collision-detection-cuda
	// https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
	glm::vec3 v = data()->physics.velocity + other->component->data()->physics.velocity;
	glm::vec3 normal;
	float distance;
	if (OWUtils::isZero(v))
		return;
	glm::vec3 c1 = other->component->data()->boundingBox.minPoint() - glm::vec3(2, 2, 2);
	OWRay r2(data()->boundingBox.center(), v);
	if (!r2.intersects(other->component->data()->boundingBox, normal, distance))
	{
		return;
		// throw NMSLogicException("Object [" + name() + "] collided by ray intersection failed");
	}
	v = glm::normalize(v);
	// https://stackoverflow.com/questions/573084/how-to-calculate-bounce-angle
	// https://3dkingdoms.com/weekly/weekly.php?a=2
	constexpr float notPerfectBounce = 1.0f; // A perfect bounce
	glm::vec3 reboundDir = notPerfectBounce * (v - 2 * glm::dot(v, normal) * normal);
	glm::vec3 ourCenter = data()->boundingBox.center();
	glm::vec3 otherCenter = other->component->data()->boundingBox.center();
	//float dist = glm::length(ourCenter - otherCenter);
	//float fullTimeStep = glm::length(previousPosition() - position()) / glm::length(mCurrent.mVelocity);
	//float curtailedTimeStep = glm::length(distance) / glm::length(mCurrent.mVelocity);
	// jfw prorataDistance is wrong.
	float len = glm::length(ourCenter - otherCenter);
	//float len2 = glm::length(ourCenter - position());
	float prorataTimeStep = distance / len;// curtailedTimeStep / fullTimeStep;
	translate(prorataTimeStep * glm::length(data()->physics.velocity) * glm::normalize(reboundDir));
	velocity(velocity() + reboundDir * velocity());
}

void OLDSceneComponent::render(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	const glm::vec3& cameraPos,
	RenderTypes::ShaderMutator renderCb,
	RenderTypes::ShaderResizer resizeCb) 
{
	std::string ss = name();
	if (ss == "Rope Ends")
	{
		ss = "Rope Ends";
	}
	if (data()->physics.visibility > 0.001f)
	{
		OLDPhysicsDataImp* imp = &data()->physics;
		const glm::mat4 I(1.0f);
		float len = glm::length(imp->mRotate);
		glm::vec3 norm = glm::normalize(imp->mRotate);
		glm::mat4 r = OWUtils::isZero(len) ? I : glm::rotate(model, len, norm);
		glm::mat4 s = glm::scale(I, imp->mScale);
		glm::mat4 t = glm::translate(I, imp->mTranslate);
		glm::mat4 _model = t * r * s;
		mRenderer->render(proj, view, _model, cameraPos, renderCb, resizeCb);
		if (mBoundingBoxRenderer != nullptr)
		{
			glm::vec3 newScaling(0);
			const AABB& orig = mOriginalBoundingBox;
			const AABB& current = data()->boundingBox;
			newScaling.x = glm::abs(((current.maxPoint().x - current.minPoint().x)) / (orig.maxPoint().x - orig.minPoint().x));
			newScaling.y = glm::abs(((current.maxPoint().y - current.minPoint().y)) / (orig.maxPoint().y - orig.minPoint().y));
			newScaling.z = glm::abs(((current.maxPoint().z - current.minPoint().z)) / (orig.maxPoint().z - orig.minPoint().z));
			const glm::mat4 I2(1.0f);
			glm::mat4 s2 = glm::scale(model, newScaling);
			glm::mat4 t2 = glm::translate(I2, translation());
			glm::mat4 model2 = t2 * s2;
			mBoundingBoxRenderer->render(proj, view, model2, cameraPos, renderCb, resizeCb);
		}
	}
}

