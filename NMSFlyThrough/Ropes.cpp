#include "Ropes.h"
#include <string>

#include <Helpers/ShaderFactory.h>
#include <Renderers/MeshRenderer.h>

#include <Component/MeshComponent.h>
#include "./../NMSFlyThrough/rope_interface_test.h"
#include "./../NMSFlyThrough/rope_quick.h"
#include "RopeNormaliser.h"


bool initTestFunctions();
bool testConstructionSet();
bool testEnums();
bool testLibraryName();
bool runImportTests();


static bool mRopeDLLOk = false;
static std::string gRopeEnds = "RopeEnds";
static std::string gRopeLines = "RopeLines";
static std::string gRopeSurfaces = "RopeSurfaces";

Rope::Rope(Scene* _scene, const std::string& _name, const OWRopeData& _data)
	: OWActorSingle(_scene, _name), mData(_data)
{
}

void Rope::doSetup()
{
	if (!prepare())
		return; //fail to init DLL
	const OWTextComponentData& tcd = mData.labelTextData;
	const OWRopeDataImp& rd = mData.ropeData;
	prepareRope(rd.ropeDBId, rd.ropeZoom.x, rd.ropeZoom.y, rd.numDepthLayers);
	prepareText(tcd.fontHeight, tcd.fontSpacing, glm::vec2(1));
	{
		Shader* lineShader = new Shader("");
		lineShader->setStandardUniformNames("pvm");

		OWActorSingle::SingleSceneElement sse;
		sse.m = createRopeLines(mPolyBuilder->slices());
		sse.c = new OWCollider(this, OWCollider::CollisionType::Box);
		sse.r = new OWMeshRenderer(lineShader, OWMeshRenderer::DRAW_MULTI);
		mRopeLinesElementIndex = this->addComponents(sse);
	}
	{
		Shader* wireShader = new Shader();
		wireShader->loadShaders("Wires.v.glsl",
			"Wires.f.glsl",
			ShaderFactory::boilerPlateGeometryShader());
		wireShader->setStandardUniformNames("", "projection", "view", "model");
		auto pointRender = [](
			const glm::mat4& OW_UNUSED(proj),
			const glm::mat4& OW_UNUSED(view),
			const glm::mat4& OW_UNUSED(model),
			const glm::vec3& cameraPos,
			const Shader* shader)
			{
				shader->use();
				shader->setVector4f("lightColor", OWUtils::colour(OWUtils::SolidColours::WHITE));
				shader->setVector4f("objectColor", glm::vec4(0.90f, 0.91f, 0.98f, 1.0f)); // silver
				shader->setVector3f("viewLightPos", glm::vec3(160.0f, 60.0f, 50.0f));
				//shader->setVector3f("viewLightPos", cameraPos);
			};
		wireShader->appendMutator(pointRender);

		OWActorSingle::SingleSceneElement sse;
		sse.m = createRopeSurfaces(mPolyBuilder->slices());
		sse.c = new OWCollider(this, OWCollider::CollisionType::Box);
		sse.r = new OWMeshRenderer(wireShader, OWMeshRenderer::DRAW_ARRAYS);
		mRopeSurfacesElementIndex = this->addComponents(sse);
	}
	{
		Shader* lineShader = new Shader("");
		lineShader->setStandardUniformNames("pvm");
		OWActorSingle::SingleSceneElement sse;
		sse.m = createRopeEnds(mPolyBuilder->slices());
		sse.c = new OWCollider(this, OWCollider::CollisionType::Box);
		sse.r = new OWMeshRenderer(lineShader, OWMeshRenderer::DRAW_MULTI);
		mRopeEndsElementIndex = this->addComponents(sse);
	}
	const OWRopeVisibilityData& vd = mData.ropeVisibility;
	makeVisible(vd.ends, vd.lines, vd.surfaces, vd.strandLabels, vd.bannerLabel);
	OWActorSingle::doSetup();
}

void Rope::makeVisible(bool _ends, bool _lines, bool _surfaces, bool _strandLabels, bool _bannerLabel)
{
	mElements[mRopeEndsElementIndex].m->active(_ends);
	mElements[mRopeLinesElementIndex].m->active(_lines);
	mElements[mRopeSurfacesElementIndex].m->active(_surfaces);
	mElements[mRopeBannerElementIndex].m->active(_bannerLabel);
	mElements[mRopeLabelsElementIndex].m->active(_strandLabels);
}

bool Rope::prepare()
{
	if (!mRopeDLLOk)
		mRopeDLLOk = initRopes();
	return mRopeDLLOk;
}

bool Rope::initRopes()
{
	bool ok = initInterfaceUtils() && initTestFunctions() && initQuickExterns();
	if (ok)
	{
		ok = runImportTests();
	}
	return ok;
}

void Rope::prepareRope(int ropeNum, int width, int height, int numDepthLayers)
{
	RopeBuf* pointSourceBuffer = calcQuickRope(ropeNum, width, height, numDepthLayers);
	mPolyBuilder = new PolygonBuilder();
	mPolyBuilder->get(pointSourceBuffer);
	std::pair<glm::vec3, glm::vec3> b = mPolyBuilder->bounds();
	mBounds = AABB(b.first, b.second);
}

void Rope::prepareText(int fontHeight, const glm::vec2& textSpacing, const glm::vec2& textScale)
{
	makeLabels(textSpacing, textScale);
	makeBanner(mData.bannerTextData.text, fontHeight, textSpacing * 10.0f, textScale);
}

void Rope::makeBanner(const std::string& s, int height,
	const glm::vec2& _spacing, const glm::vec2& scale,
	const std::string& _font,
	const glm::vec4& colour)
{
	OWTextComponentData td;
	td.tdt = OWTextComponentData::TextDisplayType::Dynamic;
	td.text = s;
	td.fontName = _font;
	td.fontHeight = height;
	td.fontSpacing = _spacing;
	td.colour = colour;
	OWActorSingle::SingleSceneElement sse;
	sse.m = new OWTextComponent(this, "Rope Banner", td);
	sse.p = new OWPhysics();
	sse.t = new OWTransform(transform(), glm::vec3(0), glm::vec3(scale, 1.0f));
	sse.r = new OWMeshRenderer("DynamicText.json");
	this->addComponents(sse);
}

void Rope::makeLabels(const glm::vec2& textSpacing, const glm::vec2& textScale)
{
	OWActorMulti* multipleTexts = new OWActorMulti(this->scene(), "Rope Labels", this);
	multipleTexts->renderer(new OWMeshRenderer("DynamicText.json"));
	for (const PolygonBuilder::SliceId& si : mPolyBuilder->labels())
	{
		OWTextComponentData td;
		td.tdt = OWTextComponentData::TextDisplayType::Dynamic;
		td.text = std::to_string(si.id);
		td.fontHeight = mData.labelTextData.fontHeight;
		td.fontSpacing = textSpacing * 10.0f;
		OWActorMulti::MultiSceneElement mse;
		mse.m = new OWTextComponent(this, td.text, td);
		mse.p = new OWPhysics();
		mse.t = new OWTransform(this->transform(), si.pos);
		multipleTexts->addComponents(mse);
	}
}


OWMeshComponent* Rope::createRopeEnds(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires)
{
	OWMeshComponent* mvd = new OWMeshComponent(this, gRopeEnds);

	// Prepare the wire cross sections
	std::vector<glm::vec3> coords;
	for (auto& slice: threeDWires)
	{
		for (auto& polygon : slice)
		{
			MeshData lineData;
			lineData.setVertices(polygon, GL_LINE_LOOP);
			mvd->add(lineData);
		}
		break;
	}
	return mvd;
}

OWMeshComponent* Rope::createRopeLines(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires)
{
	OWMeshComponent* mc = new OWMeshComponent(this, gRopeLines);

	bool drawCenters = false;
	bool drawLines = true;

	size_t numLayers = threeDWires.size();
	size_t numWiresInEachLayer = threeDWires[0].size();
	size_t numPointsPerSlice = threeDWires[0][0].size() + 10;
	std::vector< std::vector<glm::vec3>> ropeLines;
	std::vector< std::vector<glm::vec3>> surfaces;
	for (size_t eachWire = 0; eachWire < numWiresInEachLayer; eachWire++)
	{
		if (drawCenters)
		{
			std::vector<glm::vec3> line;
			for (int layer = 0; layer < numLayers; layer++)
			{
				std::vector<glm::vec3>& slice = threeDWires[layer][eachWire];
				line.push_back(AABB(slice).center());
			}
			ropeLines.push_back(line);
		}
		if (drawLines)
		{
			for (int pointOnPoly = 0; pointOnPoly < numPointsPerSlice; pointOnPoly++)
			{
				std::vector<glm::vec3> line;
				for (int layer = 0; layer < numLayers; layer++)
				{
					if (pointOnPoly < threeDWires[layer][eachWire].size())
					{
						glm::vec3 pt = threeDWires[layer][eachWire][pointOnPoly];
						line.push_back(pt);
					}
				}
				if (line.size())
					ropeLines.push_back(line);
			}
		}
	}
	if (drawLines)
	{
		for (std::vector<glm::vec3>& aLine : ropeLines)
		{
			MeshData lineData;
			lineData.setColour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED), "colour");
			lineData.setVertices(aLine, GL_LINE_STRIP);
			mc->add(lineData);
		}
	}
	return mc;
}

glm::vec3 centerOfPolygon(const std::vector<glm::vec3>& polygon)
{
	glm::vec3 firstPolyCoord = polygon[0];
	glm::vec3 oppositePolyCoord = polygon[polygon.size() / 2];
	glm::vec3 midpoint = (firstPolyCoord + oppositePolyCoord) * 0.5f;
	return midpoint;
}

static unsigned int safeWrap(const std::vector<unsigned int>& vv, size_t ndx)
{
	int i = static_cast<int>(ndx) - static_cast<int>(vv.size());
	return i < 0 ? vv[ndx] : vv[i];
}

OWMeshComponent* Rope::createRopeSurfaces(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires)
{
	OWMeshComponent* mc = new OWMeshComponent(this, gRopeSurfaces);

	// Prepare the wire cross sections
	size_t numLayers = threeDWires.size();

	// The number of wires for each layer is the same
	size_t numWires = threeDWires[0].size();
	// This holds the points that go to the gpu
	std::vector<glm::vec3> triAnglePoints;

	/*
	* For each wire mirror the structure of threeDWires, but populate
	* with the indices of the corresponding point pushed back into triAnglePoints.
	*/
	std::vector<std::vector<std::vector<unsigned int>>> tempWireIndices(threeDWires.size());
	for (int i = 0; i < threeDWires.size(); i++)
	{
		tempWireIndices[i] = std::vector<std::vector<unsigned int>>(threeDWires[i].size());
		for (int j = 0; j < threeDWires[i].size(); j++)
		{
			tempWireIndices[i][j] = std::vector<unsigned int>(threeDWires[i][j].size());
			for (unsigned int pointOnPoly = 0; pointOnPoly < threeDWires[i][j].size(); pointOnPoly++)
			{
				unsigned int ndx = RopeNormaliser::append(triAnglePoints, threeDWires[i][j][pointOnPoly]);
				tempWireIndices[i][j][pointOnPoly] = ndx;
			}
		}
	}

	// Remember the centroid of each wire end so we can draw triangles on the ends.
	std::vector<std::vector<unsigned int>> centroidIndex(numWires, std::vector<unsigned int>(2, 0));
	// Populate the indice vectors
	for (int eachWire = 0; eachWire < threeDWires[0].size(); eachWire++)
	{
		/*
		* Find the rough centroids of the first and last polygon slices
			(layer == 0 and layer == numLayers - 1) respectively. We use these to
		* triangulate the surfaces of the end points.
		*/
		centroidIndex[eachWire][0] = RopeNormaliser::append(triAnglePoints,
			centerOfPolygon(threeDWires[0][eachWire]));

		// Center of the bottom slice
		centroidIndex[eachWire][1] = RopeNormaliser::append(triAnglePoints,
			centerOfPolygon(threeDWires[numLayers - 1][eachWire]));
	}

	RopeNormaliser rn(&triAnglePoints);
	// Now build the triangles
	for (int eachWire = 0; eachWire < tempWireIndices[0].size(); eachWire++)
	{
		for (int layer = 0; layer < numLayers; layer++)
		{
			size_t polySize = tempWireIndices[layer][eachWire].size();
			for (size_t pointOnPoly = 0; pointOnPoly < polySize; pointOnPoly++)
			{
				if (layer < (numLayers - 1))
				{
					if (layer == 0)
					{
						// Form a triangle on the flat surface of the endpoint
						rn.appendTriangle(
							centroidIndex[eachWire][0],
							safeWrap(tempWireIndices[layer][eachWire], pointOnPoly),
							safeWrap(tempWireIndices[layer][eachWire], pointOnPoly + 1));
					}
					// Now for the triangles down the side of the wire
					rn.appendTriangle(
						safeWrap(tempWireIndices[layer][eachWire], pointOnPoly + 1),
						safeWrap(tempWireIndices[layer][eachWire], pointOnPoly),
						safeWrap(tempWireIndices[layer + 1][eachWire], pointOnPoly));

					rn.appendTriangle(
						safeWrap(tempWireIndices[layer][eachWire], pointOnPoly + 1),
						safeWrap(tempWireIndices[layer + 1][eachWire], pointOnPoly),
						safeWrap(tempWireIndices[layer + 1][eachWire], pointOnPoly + 1));

					// The next layers polygon may have more points than this layer.
					// If we are the last point in this layer and the next layer has 
					// more points then fill the gaps.
					if ((pointOnPoly + 1 == polySize) &&
						(pointOnPoly + 1 < tempWireIndices[layer + 1][eachWire].size()))
					{
						size_t a = pointOnPoly + 1;
						while (a < tempWireIndices[layer + 1][eachWire].size())
						{
							rn.appendTriangle(
								safeWrap(tempWireIndices[layer + 1][eachWire], a + 1),
								safeWrap(tempWireIndices[layer][eachWire], pointOnPoly + 1),
								safeWrap(tempWireIndices[layer + 1][eachWire], a));
							a++;
						}
					}
				}
				else if (layer == (numLayers - 1))
				{
					// Now form triangles on the flat surface of the end of the wire.
					rn.appendTriangle(
						safeWrap(
							tempWireIndices[layer][eachWire], pointOnPoly + 1),
						tempWireIndices[layer][eachWire][pointOnPoly],
						centroidIndex[eachWire][1]);

				}
			}
			//	break;
		}
	}

	rn.createNormals(triAnglePoints, 1, 2);
	MeshData lineData;
	lineData.setVertices(triAnglePoints, GL_TRIANGLES);
	lineData.setIndices(rn.mIndexBuffer, GL_TRIANGLES);
	lineData.setPolygonMode(GL_FILL);
	return mc;
}
