#pragma once
#include <vector>

#include <Core/OWActor.h>
#include <Geometry/GeometricShapes.h>
#include "PolygonBuilder.h"

class OWSceneComponent;
class Rope: public OWActor
{
private:
	bool initRopes();
	PolygonBuilder* mPolyBuilder = nullptr;
	
	std::pair<glm::vec3, glm::vec3> mMinMax = GeometricShapes::minMaxBox;
	AABB mBounds;
public:
	Rope(Scene* _owner, const glm::vec3& _position)
		: OWActor(_owner, _position) {}
	void visualComponents(bool _ends, bool _lines, bool _surfaces, bool _labels);
	bool prepare();
	void prepareRope(int ropeNum, int width, int height, int numDepthLayers,
		int fontHeight, const glm::vec2& textSpacing, const glm::vec2& textScale);
	void lines();
	void ends();
	void surfaces();
	const AABB& bounds() const { return mBounds; }
private:
	void makeBanner(const std::string& s, int height,
		const glm::vec2& _spacing, const glm::vec2& scale,
		const std::string& f = "arial.ttf",
		const glm::vec4& col = { 0.0, 0.0, 0.0, 1.0f });
	void labels(const glm::vec2& textSpacing = { 0,0 }, const glm::vec2& textScale = { 0,0 });
	OWSceneComponent* createRopeEnds(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	OWSceneComponent* createRopeLines(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	OWSceneComponent* createRopeSurfaces(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	bool mLines = false;
	bool mSurfaces = false;
	bool mEnds = false;
	bool mLabels = false;
};
