#pragma once

#include <limits>

#include <glm/glm.hpp>

#include <Geometry/BoundingBox.h>

#include "NMSScene.h"

/*
	An implementation of a Scene for the NMS game.
*/

struct ButtonData;
class MeshDataInstance;

class NMSSplashScene : public NMSScene
{
public:
	static AABB mWindowBounds;
	static OWUtils::Float mSpeed;
	ButtonData* mButtonData = nullptr;
	MeshDataInstance* mStarData = nullptr;
	glm::vec2 mStarRadius = glm::vec2(0);

	NMSSplashScene(const Movie* movie);
	std::string name() const { return "Splash"; }
	virtual void doRenderScene(const glm::mat4& proj, const glm::mat4& view,
					const glm::vec3& cameraPos) override;
	void activate(const std::string& previousScene, 
				  Camera* camera, unsigned int callCount) override;
	bool processUserCommands(const UserInput::AnyInput& userInput, std::string& nextScene, Camera* camera) override;
	void deActivate(const Camera* camera) override;
protected:
	void doSetupScene() override;
private:
};

