#pragma once
#include <vector>

#include <Actor/OWActor.h>
#include <Geometry/GeometricShapes.h>
#include <Helpers/Shader.h>
#include <Component/TextComponent.h>
#include "PolygonBuilder.h"

struct OWRopeDataImp
{
	unsigned int ropeDBId;
	glm::vec2 ropeZoom;
	unsigned int numDepthLayers = 30;
	glm::vec4 colour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED);
};

struct OWRopeVisibilityData
{
	bool ends = true;
	bool lines = true;
	bool surfaces = true;
	bool strandLabels = false;
	bool bannerLabel = true;
	glm::vec4 colour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED);
};

struct OWRopeData
{
	OWRopeDataImp ropeData;
	OWTextComponentData labelTextData;
	OWTextComponentData bannerTextData;
	OWRopeVisibilityData ropeVisibility;
};

class Rope: public OWActorSingle
{
private:
	bool initRopes();
	OWRopeData mData;
	PolygonBuilder* mPolyBuilder = nullptr;
	
protected:
	void doSetup() override;
public:
	Rope(Scene* _scene, const std::string& _name, const OWRopeData& _data);
private:
	AABB mBounds;
	bool prepare();
	void prepareRope(int ropeNum, float width, float height, int numDepthLayers);
	void prepareText(int fontHeight, const glm::vec2& textSpacing, const glm::vec2& textScale);
	void prepareVisibility(bool _ends, bool _lines, bool _surfaces, bool _strandLabels, bool _bannerLabel);
	void makeBanner(const std::string& s, int height,
		const glm::vec2& _spacing, const glm::vec2& scale,
		const std::string& f = "arial.ttf",
		const glm::vec4& col = { 0.0, 0.0, 0.0, 1.0f });
	void makeLabels(const glm::vec2& textSpacing = { 0,0 }, const glm::vec2& textScale = { 0,0 });
	OLDSceneComponent* createRopeEnds(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	OLDSceneComponent* createRopeLines(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	OLDSceneComponent* createRopeSurfaces(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	bool mLines = true;
	bool mSurfaces = true;
	bool mEnds = true;
	bool mStrandLabels = false;
	bool mBannerLabel = false;
};
