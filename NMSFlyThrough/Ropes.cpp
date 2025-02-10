#include "Ropes.h"
#include <string>

#include <Helpers/ShaderFactory.h>
#include <Helpers/MeshDataLight.h>
#include <Renderers/VAOBuffer.h>

#include <Component/MeshComponentVAO.h>
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

Rope::Rope(Scene* _scene, OWRopeScript* _script)
	: OWActor(_scene, _script) 
{
}

void Rope::doInit()
{
	if (!prepare())
		return; //fail to init DLL
	const OWRopeDataImp* rd = &constData()->ropeData;
	prepareRope(rd->ropeDBId, rd->ropeZoom.x, rd->ropeZoom.y, rd->numDepthLayers);
	prepareText(rd->textData.fontHeight, rd->textData.fontSpacing, rd->textData.fontScale);
	createRopeLines(mPolyBuilder->slices());
	createRopeSurfaces(mPolyBuilder->slices());
	createRopeEnds(mPolyBuilder->slices());
	const OWRopeVisibilityData* vd = &constData()->ropeVisibility;
	prepareVisibility(vd->ends, vd->lines, vd->surfaces, vd->strandLabels, vd->bannerLabel);
	OWActor::doInit();
}

void Rope::prepareVisibility(bool _ends, bool _lines, bool _surfaces, bool _strandLabels, bool _bannerLabel)
{
	static int firstTime = true;
	if ((firstTime) || (mEnds != _ends))
	{
		auto toggleRender = [_ends](OWSceneComponent* sc)
			{
				if (sc->name().find(gRopeEnds) != std::string::npos)
				{
					sc->visibility(_ends ? 1.0f: 0.0f);
				}
			};
		traverse(toggleRender);
		mEnds = _ends;
	}

	if ((firstTime) || (mLines != _lines))
	{
		auto toggleRender = [_lines](OWSceneComponent* sc)
			{
				if (sc->name().find(gRopeLines) != std::string::npos)
				{
					sc->visibility(_lines ? 1.0f : 0.0f);
				}
			};
		traverse(toggleRender);
		mLines = _lines;
	}
	
	if ((firstTime) || (mSurfaces != _surfaces))
	{
		auto toggleRender = [_surfaces](OWSceneComponent* sc)
			{
				if (sc->name().find(gRopeSurfaces) != std::string::npos)
				{
					sc->visibility(_surfaces ? 1.0f : 0.0f);
				}
			};
		traverse(toggleRender);
		mSurfaces = _surfaces;
	}

	if ((firstTime) || (_strandLabels != mStrandLabels))
	{
		std::string banner = "Text:" + constData()->ropeData.bannerText;
		auto toggleRender = [_strandLabels, banner](OWSceneComponent* sc)
			{
				if ((sc->name().find("Text:") != std::string::npos) && 
					(sc->name() != banner))
				{
					sc->visibility(_strandLabels ? 1.0f : 0.0f);
				}
			};
		traverse(toggleRender);
	}

	if ((firstTime) || (_bannerLabel != mBannerLabel))
	{
		std::string banner = "Text:" + constData()->ropeData.bannerText;
		auto toggleRender = [_bannerLabel, banner](OWSceneComponent* sc)
			{
				if (sc->name() == banner)
				{
					sc->visibility(_bannerLabel ? 1.0f : 0.0f);
				}
			};
		traverse(toggleRender);
	}
	if (firstTime)
		firstTime = false;
}

bool Rope::prepare()
{
	if (!mRopeDLLOk)
		mRopeDLLOk = initRopes();
	return mRopeDLLOk;
}

bool Rope::initRopes()
{
	name("Ropes");
	bool ok = initInterfaceUtils() && initTestFunctions() && initQuickExterns();
	if (ok)
	{
		ok = runImportTests();
	}
	return ok;
}

void Rope::prepareRope(int ropeNum, float width, float height, int numDepthLayers)
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
	makeBanner(constData()->ropeData.bannerText, fontHeight, textSpacing * 10.0f, textScale);
}

void Rope::makeBanner(const std::string& s, int height,
	const glm::vec2& _spacing, const glm::vec2& scale,
	const std::string& f,
	const glm::vec4& col)
{
	TextComponentData* td = new TextComponentData();
	td->textData.tdt = TextData::TextDisplayType::Dynamic;
	td->textData.text = s;
	td->textData.fontHeight = height;
	td->textData.fontSpacing = _spacing;
	td->textData.fontScale = scale;
	TextComponent* textData = new TextComponent(this, td);
	textData->init();
}

void Rope::makeLabels(const glm::vec2& textSpacing, const glm::vec2& textScale)
{
	for (const PolygonBuilder::SliceId& si : mPolyBuilder->labels())
	{
		TextComponentData* td = new TextComponentData();
		td->textData.tdt = TextData::TextDisplayType::Dynamic;
		td->textData.text = std::to_string(si.id);
		td->textData.fontHeight = constData()->ropeData.labelFontHeight;
		td->textData.fontSpacing = textSpacing * 10.0f;
		td->textData.fontScale = textScale;
		glm::mat4 m(1.0f);
		td->physics.localMatrix = glm::translate(m, si.pos);
		TextComponent* tc = new TextComponent(this, td);
		tc->init();
	}
}


OWSceneComponent* Rope::createRopeEnds(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires)
{
	MeshComponentVAOData* mvd = new MeshComponentVAOData();
	mvd->shaderData.PVMName = "pvm";
	mvd->meshData = VAOBuffer(new Shader(&mvd->shaderData), VAOBuffer::DRAW_MULTI);

	// Prepare the wire cross sections
	std::vector<glm::vec3> coords;
	for (auto& slice : threeDWires)
	{
		for (auto& polygon : slice)
		{
			MeshDataLight lineData;
			glm::vec4 colour = data()->colour;
			lineData.colour(colour, "colour");
			lineData.vertices(polygon, GL_LINE_LOOP);
			mvd->meshData.add(&lineData);
		}
		break;
	}
	mvd->name = gRopeEnds;
	mvd->boundingBox.render(false);
	MeshComponentVAO* m = new MeshComponentVAO(this, mvd);
	m->init();
	return m;
}

OWSceneComponent* Rope::createRopeLines(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires)
{
	MeshComponentVAOData* mvd = new MeshComponentVAOData();
	mvd->shaderData.PVMName = "pvm";
	Shader* lineShader = new Shader(&mvd->shaderData);
	lineShader->loadBoilerPlates();
	mvd->meshData = VAOBuffer(lineShader, VAOBuffer::DRAW_MULTI);

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
			MeshDataLight lineData;
			lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED), "colour");
			lineData.vertices(aLine, GL_LINE_STRIP);
			mvd->meshData.add(&lineData);
		}
	}
	mvd->name = gRopeLines;
	mvd->boundingBox.render(false);
	MeshComponentVAO* m = new MeshComponentVAO(this, mvd);
	m->init();
	return m;
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

OWSceneComponent* Rope::createRopeSurfaces(std::vector<std::vector<std::vector<glm::vec3>>>& threeDWires)
{
	MeshComponentVAOData* mvd = new MeshComponentVAOData();
	mvd->shaderData.shaderV = "Wires.v.glsl";
	mvd->shaderData.shaderF = "Wires.f.glsl";
	mvd->shaderData.PVMName = "pvm";
	mvd->shaderData.projectionName = "projection";
	mvd->shaderData.viewName = "view";
	mvd->shaderData.modelName = "model";
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
	mvd->shaderData.mutatorCallbacks.push_back(pointRender);
	Shader* wireShader = new Shader(&mvd->shaderData);
	mvd->meshData = VAOBuffer(wireShader, VAOBuffer::DRAW_ARRAYS);

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
	MeshDataLight lineData;
	lineData.vertices(triAnglePoints, GL_TRIANGLES);
	lineData.indices(rn.mIndexBuffer, GL_TRIANGLES);
	lineData.polygonMode(GL_FILL);
	//lineData.polygonMode(GL_LINE);
	mvd->meshData.add(&lineData);
	mvd->name = gRopeSurfaces;
	mvd->boundingBox.render(false);
	MeshComponentVAO* m = new MeshComponentVAO(this, mvd);
	m->init();
	return m;
}
