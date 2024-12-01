#pragma once
#include <vector>

#include <Core/Actor.h>
#include <Helpers/GeometricShapes.h>
#include "PolygonBuilder.h"

class Rope: public Actor
{
private:
	bool initRopes();
	PolygonBuilder* mPolyBuilder = nullptr;
	
	std::pair<glm::vec3, glm::vec3> mMinMax = GeometricShapes::minMaxBox;
public:
	Rope(Actor* _owner): Actor(_owner) {}
	void visualComponents(bool _ends, bool _lines, bool _surfaces);
	bool prepare();
	void prepareRope(int ropeNum, int width, int height, int numDepthLayers);
	std::vector<Actor*> labels(const glm::vec2& textSpacing = { 0,0 }, const glm::vec2& textScale = { 0,0 });
	void lines();
	void ends();
	void surfaces();
private:
	RendererBase* createRopeEnds(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	RendererBase* createRopeLines(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	RendererBase* createRopeSurfaces(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	bool mLines = false;
	bool mSurfaces = false;
	bool mEnds = false;
};
