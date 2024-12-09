#include "NMSScene.h"

#include <glm/glm.hpp>

#include <Core/Camera.h>

#include <Core/ErrorHandling.h>

//float sc = 1.0;
AABB NMSScene::mWorld = AABB (glm::vec3(-100, -100, -100), glm::vec3(100, 100, 100));
//AABB NMSScene::mWorld = AABB(glm::vec3(-500 * sc, -500 * sc, -500 * sc, 1), glm::vec3(500 * sc, 500 * sc, 500 * sc));
//AABB NMSScene::mWorld = AABB(glm::vec3(-8, -8, -8), glm::vec3(8, 8, 8, 1));
//AABB NMSScene::mWorld = AABB(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1, 1));
//AABB NMSScene::mWorld = AABB(glm::vec3(0, 0, 0), glm::vec3(0xFFF * 2, 0xFF * 2, 0xFFF * 2));
//AABB NMSScene::mWorld = AABB(glm::vec3(-0xFFF, -0xFF, -0xFFF), glm::vec3(0xFFF, 0xFF, 0xFFF));
NMSScene::NMSScene(const Movie* movie)
	:Scene(movie)
{}
