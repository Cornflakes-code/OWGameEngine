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
	glm::uvec2 ropeZoom;
	unsigned int numDepthLayers = 30;
	glm::vec4 colour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED);
	glm::vec2 bannerTextScale;
	glm::vec2 labelTextScale;
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

class Rope: public OWActorDiscrete
{
public:
	Rope(Scene* _scene, const std::string& _name);
	void initialise(const OWRopeData& _data = OWRopeData());
private:
	OWRopeData mData;
	AABB mBounds;
	PolygonBuilder* mPolyBuilder = nullptr;
	OWSize mRopeEndsElementIndex = 0;
	OWSize mRopeLinesElementIndex = 0;
	OWSize mRopeSurfacesElementIndex = 0;
	OWSize mRopeBannerElementIndex = 0;
	OWSize mRopeLabelsElementIndex = 0;
	bool mStrandLabels = false;
	bool mBannerLabel = false;

	bool initRopes();
	bool prepare();
	void prepareRope(int ropeNum, int width, int height, int numDepthLayers);
	void makeVisible(bool _ends, bool _lines, bool _surfaces, bool _strandLabels, bool _bannerLabel);
	void createBanner(const std::string& s, int height,
		const glm::vec2& _spacing, const glm::vec2& scale,
		const std::string& _font = "arial.ttf",
		const glm::vec4& colour = { 0.0, 0.0, 0.67, 1.0f });
	void createLabels(const glm::vec2& textSpacing = { 0,0 }, const glm::vec2& textScale = { 0,0 });
	OWMeshComponent* createRopeEnds(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	OWMeshComponent* createRopeLines(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
	OWMeshComponent* createRopeSurfaces(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires);
};
