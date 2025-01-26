#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <Geometry/BoundingBox.h>
#include <Helpers/Shader.h>

#include <Actor/OWActor.h>
#include <Component/TextData.h>
#include <Component/MeshComponent.h>

class Shader;
class TextRenderer;
/*
	The NMS game is a simple fly through of Solar Systems visited while
	playing the No Man's Sky game.
*/

struct OWENGINE_API NoMansSkyData: public OWActorData
{
	std::vector<glm::vec4> instanceColours =
	{
		OWUtils::colour(OWUtils::SolidColours::BLUE),
		OWUtils::colour(OWUtils::SolidColours::GREEN),
		OWUtils::colour(OWUtils::SolidColours::YELLOW),
		OWUtils::colour(OWUtils::SolidColours::RED),
		OWUtils::colour(OWUtils::SolidColours::MAGENTA),
		OWUtils::colour(OWUtils::SolidColours::CYAN)
	};
	TextData* textData;
	MeshComponentData* meshData;
	ShaderData* starShader;
	std::string name = "NMS Map";
	AABB NMSSize = AABB(glm::vec3(-0x7FF, -0x7F, -0x7FF),
		glm::vec3(0x7FF, 0x7F, 0x7FF));
	glm::vec4 gridColour = { 0, 1.0, 0.5, 1 };
	glm::u32vec3 gridSizes = { 0xAA, 0xAA, 0xAA };
	glm::vec2 starRadius = { 4.0, 4.0 };
	float scaleNMStoWorld = 0.0488519780f;// world.size().x / NMSSize.size().x;
	int numStars = 500000;
};

class OWENGINE_API NMSScript: public OWActorScript
{
protected:
public:
	friend class OWActor;
};

class NoMansSky: public OWActor
{
	glm::vec2 mStarRadius;
	std::vector<glm::vec3> mRandomMinorStars;
	std::vector<glm::vec4> mStarPositions;
	std::vector<glm::vec4> mStarColours;
	std::vector<glm::vec3> mGrid;

	void loadStars(const std::string& fileName, 
				   const AABB& nmsSpace,
				   float scaleToWorld);
	void createGrid(const AABB& nmsSpace,
					const glm::u32vec3& gridSizes,
					float scaleToWorld);
	std::vector<glm::vec3> createRandomVectors(const AABB& world,
					unsigned int count, float scaleToWorld);
protected:
	virtual NoMansSkyData* data()
	{
		return static_cast<NoMansSkyData*>(script()->data());
	}
public:
	NoMansSky(Scene* _owner, NMSScript* _data);
	virtual const NoMansSkyData* data() const
	{
		return static_cast<const NoMansSkyData*>(script()->data());
	}
	void setUp(const std::string& fileName, const AABB& world);
	void readSaveFile(const std::string& saveFileMeta, 
			const std::string& saveFile);
};