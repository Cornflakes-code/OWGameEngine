#include "Ropes.h"
#include <string>

#include <Helpers/ShaderFactory.h>
#include <Helpers/Transform.h>
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

Rope::Rope(Scene* _scene, const std::string& _name)
	: OWActorDiscrete(_scene, _name)
{
}

void Rope::initialise(const OWRopeData& _data)
{
	mData = _data;
	if (!prepare())
		return; //fail to init DLL
	transform(new OWTransform());
	const OWTextComponentData& tcd = mData.labelTextData;
	const OWRopeDataImp& rd = mData.ropeData;
	prepareRope(rd.ropeDBId, rd.ropeZoom.x, rd.ropeZoom.y, rd.numDepthLayers);
	if (true)
	{
		createLabels(tcd.fontSpacing, rd.labelTextScale);
	}
	if (true)
	{
		createBanner(mData.bannerTextData.text, mData.bannerTextData.fontHeight, 
			tcd.fontSpacing * 10.0f, mData.ropeData.bannerTextScale);
	}
	if (true)
	{
		Shader* lineShader = new Shader("");
		lineShader->setStandardUniformNames("pv");
		OWActorDiscrete::DiscreteEntity sse;
		sse.colour = OWUtils::colour(OWUtils::SolidColours::RED);
		sse.mesh = createRopeEnds(mPolyBuilder->slices());
		sse.coll = new OWCollider(this, OWCollider::CollisionType::Box);
		sse.rend = new OWMeshRenderer(lineShader, {}, GPUBufferObject::BufferStyle::SSBO);
		mRopeEndsElementIndex = this->addComponents(sse);
	}
	if (true)
	{
		Shader* lineShader = new Shader("");
		lineShader->setStandardUniformNames("pv");

		OWActorDiscrete::DiscreteEntity sse;
		sse.colour = OWUtils::colour(OWUtils::SolidColours::RED);
		sse.mesh = createRopeLines(mPolyBuilder->slices());
		sse.coll = new OWCollider(this, OWCollider::CollisionType::Box);
		sse.rend = new OWMeshRenderer(lineShader, {}, GPUBufferObject::BufferStyle::SSBO);
		mRopeLinesElementIndex = this->addComponents(sse);
	}
	if (true)
	{
		Shader* wireShader = new Shader();
		wireShader->loadShaders("Wires.v.glsl",
			"Wires.f.glsl",
			ShaderFactory::boilerPlateGeometryShader());
		wireShader->setStandardUniformNames("", "projection", "view");
		auto pointRender = [](
			const glm::mat4& OW_UNUSED(proj),
			const glm::mat4& OW_UNUSED(view),
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

		OWActorDiscrete::DiscreteEntity sse;
		sse.colour = OWUtils::colour(OWUtils::SolidColours::WHITE);
		sse.mesh = createRopeSurfaces(mPolyBuilder->slices());
		sse.coll = new OWCollider(this, OWCollider::CollisionType::Box);
		sse.rend = new OWMeshRenderer(wireShader, { GPUBufferObject::BufferType::Model, GPUBufferObject::BufferType::Colour },
			GPUBufferObject::BufferStyle::SSBO);
		mRopeSurfacesElementIndex = this->addComponents(sse);
	}
	const OWRopeVisibilityData& vd = mData.ropeVisibility;
	makeVisible(vd.ends, vd.lines, vd.surfaces, vd.strandLabels, vd.bannerLabel);
}

void Rope::makeVisible(bool _ends, bool _lines, bool _surfaces, bool _strandLabels, bool _bannerLabel)
{
	getMeshComponent(mRopeEndsElementIndex)->active(_ends);
	getMeshComponent(mRopeLinesElementIndex)->active(_lines);
	getMeshComponent(mRopeSurfacesElementIndex)->active(_surfaces);
	getMeshComponent(mRopeLabelsElementIndex)->active(_strandLabels);
	getMeshComponent(mRopeBannerElementIndex)->active(_bannerLabel);
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

void Rope::createBanner(const std::string& s, int height,
	const glm::vec2& _spacing, const glm::vec2& scale,
	const std::string& _font,
	const glm::vec4& colour)
{
	OWRenderTypes::DrawType tdt = OWRenderTypes::DrawType::TwoDStatic;
	OWActorNCom1Ren* multipleTexts = new OWActorNCom1Ren(this->scene(), "Rope Banner", this);
	{
		OWTransform* trans = new OWTransform();
		trans->parentTransform(transform());
		multipleTexts->transform(trans);
		Shader* shader = new Shader("textStaticBillboard.v.glsl", "text.f.glsl", "");
		shader->setStandardUniformNames("pv");
		shader->appendMutator(OWTextComponent::shaderMutator(tdt));
		multipleTexts->addRenderer(new OWMeshRenderer(shader,
				{ GPUBufferObject::BufferType::Position, 
					GPUBufferObject::BufferType::Colour, 
					GPUBufferObject::BufferType::BillboardSize },
			GPUBufferObject::BufferStyle::SSBO));
	}
	for(int i = 0; i < 1; i++) // could do many
	{
		OWTextComponentData td;
		td.tdt = tdt;
		td.text = s;
		td.fontName = _font;
		td.fontHeight = height;
		td.fontSpacing = _spacing;
		OWActorNCom1Ren::NCom1RenElement elm;
		elm.mesh = new OWTextComponent(multipleTexts, "Rope Banner", td);
		OWTransform* trans = new OWTransform({ glm::vec3(0, 0, 0), glm::vec3(0.02, 0.02, 1) });
		trans->parentTransform(multipleTexts->transform());
		elm.physics = new OWPhysics(trans);
		elm.colour = colour;
		elm.coll = new OWCollider(multipleTexts, OWCollider::CollisionType::Box);
		multipleTexts->addComponents(elm);
	}
}

void Rope::createLabels(const glm::vec2& textSpacing, const glm::vec2& textScale)
{
	std::string jfwparagraph =

		std::string("I work on a software who need to print a lot of texts on the screen, around 200 to ")
		+ "400 strings and a lot unique character(some objects are represented by a character), "
		+ "the software is already drawing a lot of things using OpenGL.I already worked around "
		+ "with text rendering and I'm able to render text, but drawing more than 200 string using "
		+ "200 draw call lead in a performance issue.";

	OWRenderTypes::DrawType tdt = OWRenderTypes::DrawType::TwoDDynamic;
	OWActorNCom1Ren* multipleTexts = new OWActorNCom1Ren(this->scene(), "Rope Labels", this);
	OWTransform* trans = new OWTransform(glm::vec3(0, 0, 0));
	trans->parentTransform(transform());
	multipleTexts->transform(trans);
	Shader* shader = new Shader("DynamicText.json");
	shader->appendMutator(OWTextComponent::shaderMutator(tdt));
	multipleTexts->addRenderer(new OWMeshRenderer(shader,
		{ GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour, GPUBufferObject::BufferType::BillboardSize },
		GPUBufferObject::BufferStyle::SSBO)); 
	for (int i = 0; i < mPolyBuilder->labels().size(); i++)
	{ 
		const PolygonBuilder::SliceId& si = mPolyBuilder->labels()[i];
		OWTextComponentData td;
		td.tdt = tdt;
		td.text = std::to_string(si.id);
		td.fontHeight = mData.labelTextData.fontHeight;
		td.fontSpacing = textSpacing * 10.0f;
		td.magicTextScaleFactor = {5.4f, 3.6f};
		OWActorNCom1Ren::NCom1RenElement elm;
		elm.mesh = new OWTextComponent(multipleTexts, td.text, td);
		OWTransform* trans = new OWTransform({ si.pos, glm::vec3(textScale, 1) });
		trans->parentTransform(multipleTexts->transform());
		elm.physics = new OWPhysics(trans);
		elm.colour = OWUtils::randomSolidColour();// OWUtils::colour(OWUtils::SolidColours::BLUE);
		elm.coll = new OWCollider(multipleTexts, OWCollider::CollisionType::Box);
		multipleTexts->addComponents(elm);
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
			mvd->setData(MeshData()
				.addVertices(polygon)
				.setModes(GL_LINE_LOOP, GL_TRIANGLES, GL_FILL));
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
			mc->setData(MeshData()
				.addVertices(aLine)
				.setColour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED), "colour")
				.setModes(GL_LINE_STRIP, GL_TRIANGLES, GL_LINE));
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
	mc->setData(MeshData()
		.addVertices(triAnglePoints)
		.addIndices(rn.mIndexBuffer)
		.setModes(GL_TRIANGLES, GL_TRIANGLES, GL_FILL));
	return mc;
}
