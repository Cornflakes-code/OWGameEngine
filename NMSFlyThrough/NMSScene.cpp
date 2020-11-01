#include "NMSScene.h"
#include <glm/glm.hpp>

#include <Core/Camera.h>
#include <Helpers/ErrorHandling.h>

//float sc = 1.0;
//AABB NMSScene::mWorld = AABB(glm::vec4(-100, -100, -100, 0), glm::vec4(100, 100, 100, 1));
//AABB NMSScene::mWorld = AABB(glm::vec4(-500 * sc, -500 * sc, -500 * sc, 1), glm::vec4(500 * sc, 500 * sc, 500 * sc, 1));
AABB NMSScene::mWorld = AABB(glm::vec4(-8, -8, -8, 0), glm::vec4(8, 8, 8, 1));
//AABB NMSScene::mWorld = AABB(glm::vec4(-1, -1, -1, 0), glm::vec4(1, 1, 1, 1));
//AABB NMSScene::mWorld = AABB(glm::vec4(0, 0, 0, 0), glm::vec4(0xFFF * 2, 0xFF * 2, 0xFFF * 2, 1));
//AABB NMSScene::mWorld = AABB(glm::vec4(-0xFFF, -0xFF, -0xFFF, 1), glm::vec4(0xFFF, 0xFF, 0xFFF, 1));
NMSScene::NMSScene(const Movie* movie)
	:Scene(movie)
{}
