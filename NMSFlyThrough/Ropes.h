#pragma once
#include <vector>

#include <Actor/OWActor.h>
#include <Geometry/GeometricShapes.h>
#include "PolygonBuilder.h"

struct OWENGINE_API OWRopeData: public OWActorData
{
	unsigned int RopeDBId;
	glm::vec2 RopeZoom;
	unsigned int NumDepthLayers = 30;
	unsigned int FontHeight = 24;
	glm::vec2 textSpacing = glm::vec2({0.00625f, 0.0125f});
	glm::vec2 textScale = glm::vec2({1.3f, 1.3f});
	bool DisplayEnds = true;
	bool DisplayLines = false;
	bool DisplaySurfaces = true;
	bool DisplayLabels = false;
	glm::vec4 Colour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED);
};

class OWENGINE_API OWRopeScript: public OWActorScript
{
	OWRopeScript(OWRopeData* _data)
		: OWActorScript(_data) {
	}
};

class OWSceneComponent;
class Rope: public OWActor
{
private:
	bool initRopes();
	PolygonBuilder* mPolyBuilder = nullptr;
	
	std::pair<glm::vec3, glm::vec3> mMinMax = GeometricShapes::minMaxBox;
	AABB mBounds;
public:
	Rope(Scene* _scene, OWRopeScript* _script);
	void visualComponents(bool _ends, bool _lines, bool _surfaces, bool _labels);
	const AABB& bounds() const { return mBounds; }
private:
	virtual OWRopeData* data() const override { return static_cast<OWRopeData*>(OWActor::data()); }
	bool prepare();
	void prepareRope(int ropeNum, int width, int height, int numDepthLayers,
		int fontHeight, const glm::vec2& textSpacing, const glm::vec2& textScale);
	void lines();
	void ends();
	void surfaces();
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
