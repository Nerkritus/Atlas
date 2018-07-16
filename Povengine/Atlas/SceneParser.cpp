#include "SceneParser.h"
#include "IO.h"

using namespace Atlas;
using namespace tinyxml2;

const char* SceneParser::SP_ASROOT = "atlasscene";
							 /// Atlas top-level scene elements
const char* SceneParser::SP_EL_BGMUSIC = "bgmusic";
const char* SceneParser::SP_EL_ENTITIES = "entities";
const char* SceneParser::SP_EL_CAM = "camera";
const char* SceneParser::SP_EL_UI = "ui";
const char* SceneParser::SP_EL_LIGHTS = "lights";
const char* SceneParser::SP_EL_SNDS = "sounds";

/// Atlas scene child elements
const char* SceneParser::SP_EL_TEXT = "text";
const char* SceneParser::SP_EL_SPRITE = "sprite";
const char* SceneParser::SP_EL_SHADER = "shader";
const char* SceneParser::SP_EL_SND = "sound";

/// Atlas common scene attributes
const char* SceneParser::SP_ATTR_ID = "id";
const char* SceneParser::SP_ATTR_POS = "position";
const char* SceneParser::SP_ATTR_COLOUR = "colour";
const char* SceneParser::SP_ATTR_NAME = "name";
const char* SceneParser::SP_ATTR_VAL = "value";
const char* SceneParser::SP_ATTR_X = "x";
const char* SceneParser::SP_ATTR_Y = "y";
const char* SceneParser::SP_ATTR_Z = "z";
const char* SceneParser::SP_ATTR_RPT = "repeat";

// Atlas physics attributes
const char* SceneParser::SP_ATTR_PHYSENABLE = "enable";
const char* SceneParser::SP_ATTR_PHYSMASS = "mass";

// Sprite attributes
const char* SceneParser::SP_ATTR_STYLE = "style";
const char* SceneParser::SP_ATTR_HALIGN = "horizontalalignment";
const char* SceneParser::SP_ATTR_VALIGN = "verticalalignment";
const char* SceneParser::SP_ATTR_WIDTH = "width";
const char* SceneParser::SP_ATTR_HEIGHT = "height";

//
//
//
Scene* SceneParser::ParseSceneFile(Scene* sceneToFill, std::string& path, Subsystems& subsystems)
{
	subsystems._log->Debug("ParseSceneFile: Starting to parse scene file: " + path);

	XMLError returnCode;

	XMLDocument doc;
	returnCode = doc.LoadFile(path.c_str());

	if (returnCode != XMLError::XML_SUCCESS) {
		subsystems._log->Error("ParseSceneFile: Failed to load scene file. Not a valid file format.");
		return nullptr;
	}

	auto root = doc.FirstChildElement(SP_ASROOT);

	if (root == nullptr) {
		subsystems._log->Error("ParseSceneFile: Failed to load scene file. It may be corrupt.");
		return nullptr;
	}

	if (sceneToFill == nullptr) {
		subsystems._log->Error("ParseSceneFile: Failed to load scene. Must supply a scene to fill.");
		return nullptr;
	}

	// TODO: Version the scene file format?

	sceneToFill->Initialise(subsystems);
	
	auto sceneElement = root->FirstChildElement();

	while (sceneElement != nullptr) {
		if (!ParseElement(sceneToFill, sceneElement, subsystems)) {
			// TODO: This currently will never be hit. Add some error checking into ParseElement
			return nullptr;
		}
		sceneElement = sceneElement->NextSiblingElement();
	}

	sceneToFill->SceneLoaded();

	subsystems._log->Debug("ParseSceneFile: Scene is loaded.");


	return sceneToFill;
}

/// <summary>
/// Determine the further action required to process an XML element within the root node and call it
/// </summary>
bool SceneParser::ParseElement(Scene* scene, XMLElement* element, Subsystems& subsystems)
{
	// Determine the element and parse it accordingly

	if (strcmp(element->Name(),SP_EL_BGMUSIC) == 0) {
		auto sound = element->FirstChildElement(SP_EL_SND);
		scene->AddBackgroundMusic(sound->FirstAttribute()->Value());
	}
	else if (strcmp(element->Name(), SP_EL_SNDS) == 0) {
		auto sound = element->FirstChildElement(SP_EL_SND);
		while (sound != nullptr) {
			scene->AddSound(sound->Attribute("file"));
			sound = sound->NextSiblingElement(SP_EL_SND);
		}
	}
	else if (strcmp(element->Name(), SP_EL_ENTITIES) == 0) {
		auto child = element->FirstChildElement();
		while (child != nullptr) {
			ParseEntity(scene, child, subsystems);
			child = child->NextSiblingElement();
		}
	}
	else if (strcmp(element->Name(), SP_EL_CAM) == 0) {
		auto child = element->FirstChildElement(SP_ATTR_POS);
		glm::vec3 pos(child->FloatAttribute(SP_ATTR_X), child->FloatAttribute(SP_ATTR_Y), child->FloatAttribute(SP_ATTR_Z));

		child = element->FirstChildElement("target");
		glm::vec3 target(child->FloatAttribute(SP_ATTR_X), child->FloatAttribute(SP_ATTR_Y), child->FloatAttribute(SP_ATTR_Z));

		scene->SetCamera(pos, target);
	}
	else if (strcmp(element->Name(), "sky") == 0) {
		ParseEntity(scene, element, subsystems);
	}
	else if (strcmp(element->Name(), SP_EL_LIGHTS) == 0) {
		auto child = element->FirstChildElement();
		while (child != nullptr) {
			ParseLight(scene, child);
			child = child->NextSiblingElement();
		}
	}
	else if (strcmp(element->Name(), SP_EL_UI) == 0) {
		ParseUI(scene, element, subsystems);
	}

	return true;
}

//
//
bool SceneParser::ParseLight(Scene* scene, XMLElement* element)
{
	if (strcmp(element->Name(), "ambient") == 0) {
		auto child = element->FirstChildElement(SP_ATTR_COLOUR);
		scene->AddLight(new Light(LightTypeEnum::LT_Ambient, child->FloatAttribute("r", 1.0f), child->FloatAttribute("g", 1.0f), child->FloatAttribute("b", 1.0f)));
	}
	else if (strcmp(element->Name(), "point") == 0) {
		auto posElement = element->FirstChildElement(SP_ATTR_POS);
		auto colourElement = element->FirstChildElement(SP_ATTR_COLOUR);

		Light* light = new Light(LightTypeEnum::LT_Point, colourElement->FloatAttribute("r", 1.0f), colourElement->FloatAttribute("g", 1.0f), colourElement->FloatAttribute("b", 1.0f),
			posElement->FloatAttribute(SP_ATTR_X, 0.0f), posElement->FloatAttribute(SP_ATTR_Y, 0.0f), posElement->FloatAttribute(SP_ATTR_Z, 0.0f));
		scene->AddLight(light);
	}
	else {
		return false;
	}
	return true;
}

/// <summary>
/// Parse an entity tag in the scene file and create the corresponding entity in the current scene
/// </summary>
bool SceneParser::ParseEntity(Scene* scene, XMLElement* element, Subsystems& subsystems)
{
	EntityCreateInfo entityInfo;

	// Get common details
	auto child = element->FirstChildElement(SP_EL_SHADER);
	if (child == nullptr) {
		entityInfo.shader = subsystems._shaderManager->GetShaderByName(SP_ATTR_COLOUR);
	}
	else {
		entityInfo.shader = subsystems._shaderManager->GetShaderByName(child->Attribute(SP_ATTR_NAME));
	}

	child = element->FirstChildElement(SP_ATTR_POS);
	if (child == nullptr) {
		entityInfo.pos = glm::vec3(0, 0, 0);
	}
	else {
		entityInfo.pos = glm::vec3(child->FloatAttribute(SP_ATTR_X), child->FloatAttribute(SP_ATTR_Y), child->FloatAttribute(SP_ATTR_Z));
	}

	child = element->FirstChildElement("uniformscale");
	if (child == nullptr) {
		entityInfo.uniformScale = 1.0f;
	}
	else {
		entityInfo.uniformScale = child->FloatAttribute(SP_ATTR_VAL);
	}

	child = element->FirstChildElement(SP_ATTR_COLOUR);
	if (child == nullptr) {
		entityInfo.colour = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	else {
		entityInfo.colour = glm::vec3(child->FloatAttribute("r"), child->FloatAttribute("g"), child->FloatAttribute("b"));
	}

	const char* id = element->Attribute(SP_ATTR_ID);
	if (id != nullptr) {
		entityInfo.id = id;
	}

	// Textures
	child = element->FirstChildElement("texture");
	if (child == nullptr) {
		entityInfo.texCount = 0;
	}
	else {
		std::string texDir = IO::GetTextureDirectory();
		auto tex = subsystems._texManager->LoadTexture(texDir + child->Attribute(SP_ATTR_NAME));
		entityInfo.textureID[entityInfo.texCount++] = tex;

		entityInfo.texRepeat = child->IntAttribute(SP_ATTR_RPT, 1);
	}

	// Physics parameters
	child = element->FirstChildElement("physics");
	if (child == nullptr) {
		entityInfo.enablePhysics = true;
		entityInfo.mass = 1.0f;
	}
	else {
		entityInfo.enablePhysics = child->BoolAttribute(SP_ATTR_PHYSENABLE, true);
		entityInfo.mass = child->FloatAttribute(SP_ATTR_PHYSMASS, 1.0f);
	}

	entityInfo.visible = element->BoolAttribute("visible", true);

	// Get the type and perform any type-specific parsing
	if (strcmp(element->Name(), "origin") == 0) {
		entityInfo.type = EntityTypeEnum::ET_Origin;
	}
	else if (strcmp(element->Name(), "plane") == 0) {
		entityInfo.type = EntityTypeEnum::ET_Plane;
	}
	else if (strcmp(element->Name(), "cone") == 0) {
		entityInfo.type = EntityTypeEnum::ET_Cone;
		entityInfo.quality = 5;
		entityInfo.uniformScale = 0.2f;
	}
	else if (strcmp(element->Name(), "sky") == 0) {
		entityInfo.type = EntityTypeEnum::ET_Skybox;

		std::string name = element->FirstAttribute()->Value();
		std::string texDir = IO::GetTextureDirectory();
		auto skyTex = subsystems._texManager->LoadTexture(texDir + name + "\\bk.jpg");
		entityInfo.texCount = 6;
		entityInfo.textureID[0] = skyTex;
		skyTex = subsystems._texManager->LoadTexture(texDir + name + "\\ft.jpg");
		entityInfo.textureID[1] = skyTex;
		skyTex = subsystems._texManager->LoadTexture(texDir + name + "\\lt.jpg");
		entityInfo.textureID[2] = skyTex;
		skyTex = subsystems._texManager->LoadTexture(texDir + name + "\\rt.jpg");
		entityInfo.textureID[3] = skyTex;
		skyTex = subsystems._texManager->LoadTexture(texDir + name + "\\up.jpg");
		entityInfo.textureID[4] = skyTex;
		skyTex = subsystems._texManager->LoadTexture(texDir + name + "\\dn.jpg");
		entityInfo.textureID[5] = skyTex;
		entityInfo.shader = subsystems._shaderManager->GetShaderByName("texture");
		entityInfo.uniformScale = 55;
		entityInfo.pos = glm::vec3(0, 0, 0);
		entityInfo.texRepeat = 1;
		entityInfo.id = name;
		entityInfo.enablePhysics = false;
		entityInfo.mass = 0;
		scene->AddEntity(subsystems._geometry->CreateEntity(entityInfo, subsystems._phys));
		return true;
	}
	else if (strcmp(element->Name(), "cube") == 0) {
		entityInfo.type = EntityTypeEnum::ET_Cube;
	}
	else if (strcmp(element->Name(), "model") == 0) {
		entityInfo.type = EntityTypeEnum::ET_Mesh;
		
		child = element->FirstChildElement("mesh");

		scene->AddMesh(std::string(child->Attribute(SP_ATTR_NAME)), entityInfo);
		return true;
	}
	else {
		return false;
	}

	scene->AddEntity(subsystems._geometry->CreateEntity(entityInfo, subsystems._phys));

	return true;
}

/// <summary>
/// Parse UI items in the scene file. UI items are all 2D text or sprites. ZOrder is determined by the order of the items in the file. Text is always on top
/// </summary>
bool SceneParser::ParseUI(Scene* scene, tinyxml2::XMLElement* element, Subsystems& subsystems)
{
	float zOrder = 0;
	auto child = element->FirstChildElement(SP_EL_SPRITE);
	while (child != nullptr) {
		if (strcmp(child->Name(), SP_EL_SPRITE) == 0) {


			auto sp = new Sprite(child->IntAttribute(SP_ATTR_X), child->IntAttribute(SP_ATTR_Y), child->IntAttribute(SP_ATTR_WIDTH), child->IntAttribute(SP_ATTR_HEIGHT), subsystems._shaderManager->GetShaderByName("texture"));
			sp->SetID(std::string(child->Attribute(SP_ATTR_ID)));

			// Textures
			auto innerChild = child->FirstChildElement("texture");
			if (innerChild != nullptr) {
				sp->SetTexture(subsystems._texManager->LoadTexture(IO::GetTextureDirectory() + innerChild->Attribute(SP_ATTR_NAME)));
			}

			sp->SetVisibility(child->BoolAttribute("visible", true));

			scene->AddSprite(sp);
		}
		child = child->NextSiblingElement();
	}

	child = element->FirstChildElement(SP_EL_TEXT);
	while (child != nullptr) {
		if (strcmp(child->Name(), SP_EL_TEXT) == 0) {
			FontStyleEnum style = ParseTextStyle(std::string(child->Attribute(SP_ATTR_STYLE)));

			auto hAlignStr = child->Attribute(SP_ATTR_HALIGN);
			auto vAlignStr = child->Attribute(SP_ATTR_VALIGN);

			auto hAlign = hAlignStr == nullptr ? TextAlignmentEnum::Left : ParseAlignment(std::string(hAlignStr));
			auto vAlign = vAlignStr == nullptr ? TextAlignmentEnum::Top : ParseAlignment(std::string(vAlignStr));

			scene->AddText(std::string(child->Attribute(SP_ATTR_ID)), std::string(child->Attribute(SP_ATTR_VAL)), child->IntAttribute(SP_ATTR_X, 0), child->IntAttribute(SP_ATTR_Y, 0), style, hAlign, vAlign, child->BoolAttribute("visible", true));
		}
		child = child->NextSiblingElement();
	}


	return true;
}

FontStyleEnum SceneParser::ParseTextStyle(std::string& style)
{
	if (style == "xsmall") {
		return FontStyleEnum::XSmall;
	}
	else if (style == "small") {
		return FontStyleEnum::Small;
	}
	else if (style == "medium") {
		return FontStyleEnum::Medium;
	}
	else if (style == "large") {
		return FontStyleEnum::Large;
	}
	else if (style == "xlarge") {
		return FontStyleEnum::XLarge;
	}
	else if (style == "title") {
		return FontStyleEnum::Title;
	}

	return FontStyleEnum::Medium;
}

TextAlignmentEnum SceneParser::ParseAlignment(std::string& alignment)
{
	if (alignment == "centre") {
		return TextAlignmentEnum::Centre;
	}
	else if (alignment == "right") {
		return TextAlignmentEnum::Right;
	}
	else if (alignment == "top") {
		return TextAlignmentEnum::Top;
	}
	else if (alignment == "bottom") {
		return TextAlignmentEnum::Bottom;
	}

	return TextAlignmentEnum::Left;
}