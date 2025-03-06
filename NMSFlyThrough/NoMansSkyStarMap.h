#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <Geometry/BoundingBox.h>
#include <Helpers/Shader.h>

#include <Actor/OWActor.h>
#include <Component/TextComponent.h>
#include <Helpers/Mesh.h>

class Shader;
class TextRenderer;
/*
	The NMS game is a simple fly through of Solar Systems visited while
	playing the No Man's Sky game.
*/
struct NoMansSkyData
{
	OWTextComponentData textData;
	ShaderData gridShader;
	ShaderData starShader;
	std::string starFile;
	std::string name = "NMS Map";
	AABB starWorld;
	AABB NMSSize = AABB(glm::vec3(-0x7FF, -0x7F, -0x7FF),
		glm::vec3(0x7FF, 0x7F, 0x7FF));
	glm::vec4 gridColour = { 0, 1.0, 0.5, 1 };
	glm::u32vec3 gridSizes = { 0xAA, 0xAA, 0xAA };
	glm::vec2 starRadius = { 4.0, 4.0 };
	float scaleNMStoWorld = 0.0488519780f;// world.size().x / NMSSize.size().x;
	int numberOfStars = 500000;
};

class NoMansSky: public OWActorSingle
{
	NoMansSkyData mData;
	glm::vec2 mStarRadius;
	std::vector<glm::vec3> mRandomMinorStars;
	std::vector<glm::vec4> mStarPositions;
	std::vector<glm::vec4> mStarColours;

	void loadStars(const std::string& fileName, 
				   const AABB& nmsSpace,
				   float scaleToWorld);
	std::vector<glm::vec3> createGrid(const AABB& nmsSpace,
					const glm::u32vec3& gridSizes,
					float scaleToWorld);
	std::vector<glm::vec3> createRandomVectors(const AABB& world,
					unsigned int count, float scaleToWorld);
protected:
	void doSetup() override;
public:
	NoMansSky(Scene* _scene, const std::string& _name, const NoMansSkyData& _data);
	void init(const std::string& fileName, const AABB& world);
	void readSaveFile(const std::string& saveFileMeta, 
			const std::string& saveFile);
};