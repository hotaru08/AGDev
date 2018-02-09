#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "CSceneGraph.h"
#include "CSceneNode.h"
#include "CUpdateTransformation.h"

#include "MapEditor\MapEditor.h"
#include "FileManager\FileManager.h"
#include "Waypoint\WaypointManager.h"
#include "Lua/CLuaInterface.h"

#include <iostream>
using namespace std;

//SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

//SceneText::SceneText(SceneManager* _sceneMgr)
//{
//	_sceneMgr->AddScene("Start", this);
//}

SceneText::~SceneText()
{
	CWaypointManager::GetInstance()->DropInstance();
	//CSpatialPartition::GetInstance()->RemoveCamera();
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	EntityManager::GetInstance()->cleartheList();

	//currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");

	//// Tell the shader program to store these uniform locations
	//{currProg->AddUniform("MVP");
	//currProg->AddUniform("MV");
	//currProg->AddUniform("MV_inverse_transpose");
	//currProg->AddUniform("material.kAmbient");
	//currProg->AddUniform("material.kDiffuse");
	//currProg->AddUniform("material.kSpecular");
	//currProg->AddUniform("material.kShininess");
	//currProg->AddUniform("lightEnabled");
	//currProg->AddUniform("numLights");
	//currProg->AddUniform("lights[0].type");
	//currProg->AddUniform("lights[0].position_cameraspace");
	//currProg->AddUniform("lights[0].color");
	//currProg->AddUniform("lights[0].power");
	//currProg->AddUniform("lights[0].kC");
	//currProg->AddUniform("lights[0].kL");
	//currProg->AddUniform("lights[0].kQ");
	//currProg->AddUniform("lights[0].spotDirection");
	//currProg->AddUniform("lights[0].cosCutoff");
	//currProg->AddUniform("lights[0].cosInner");
	//currProg->AddUniform("lights[0].exponent");
	//currProg->AddUniform("lights[1].type");
	//currProg->AddUniform("lights[1].position_cameraspace");
	//currProg->AddUniform("lights[1].color");
	//currProg->AddUniform("lights[1].power");
	//currProg->AddUniform("lights[1].kC");
	//currProg->AddUniform("lights[1].kL");
	//currProg->AddUniform("lights[1].kQ");
	//currProg->AddUniform("lights[1].spotDirection");
	//currProg->AddUniform("lights[1].cosCutoff");
	//currProg->AddUniform("lights[1].cosInner");
	//currProg->AddUniform("lights[1].exponent");
	//currProg->AddUniform("colorTextureEnabled");
	//currProg->AddUniform("colorTexture");
	//currProg->AddUniform("textEnabled");
	//currProg->AddUniform("textColor");
	//}

	// Tell the graphics manager to use the shader we just loaded
	//GraphicsManager::GetInstance()->SetActiveShader("default");

	// ------------------------------------------------- Lights ------------------------------------------------- //
	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	//currProg->UpdateInt("numLights", 1);
	//currProg->UpdateInt("textEnabled", 0);

	// ------------------------------------------------- Player Info ------------------------------------------------- //
	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// ------------------------------------------------- Camera ------------------------------------------------- //
	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// ------------------------------------------------- Meshes ------------------------------------------------- //
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateCube("gridpos", Color(1, 0, 0), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//grass_darkgreen.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//grass_lightgreen.tga");
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);

	// Initialise Ray object
	MeshBuilder::GetInstance()->GenerateRay("laser_ray");
	MeshBuilder::GetInstance()->GetMesh("laser_ray");

	// ------------------------------------------------- Zombie
	// High Level
	MeshBuilder::GetInstance()->GenerateOBJ("Body_Hi", "OBJ//Zombie//High//Body_Hi.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Head_Hi", "OBJ//Zombie//High//Head_Hi.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("RightArm_Hi", "OBJ//Zombie//High//Arm_Hi.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("LeftArm_Hi", "OBJ//Zombie//High//Arm_Hi.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("RightLeg_Hi", "OBJ//Zombie//High//Leg_Hi.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("LeftLeg_Hi", "OBJ//Zombie//High//Leg_Hi.obj");
	MeshBuilder::GetInstance()->GetMesh("Body_Hi")->textureID = LoadTGA("Image//Assignment1//ZombieBody.tga");
	MeshBuilder::GetInstance()->GetMesh("Head_Hi")->textureID = LoadTGA("Image//Assignment1//Zombie.tga");
	MeshBuilder::GetInstance()->GetMesh("RightArm_Hi")->textureID = LoadTGA("Image//Assignment1//ZombieArms.tga");
	MeshBuilder::GetInstance()->GetMesh("LeftArm_Hi")->textureID = LoadTGA("Image//Assignment1//ZombieArms.tga");
	MeshBuilder::GetInstance()->GetMesh("LeftLeg_Hi")->textureID = LoadTGA("Image//Assignment1//ZombieBody.tga");
	MeshBuilder::GetInstance()->GetMesh("RightLeg_Hi")->textureID = LoadTGA("Image//Assignment1//ZombieBody.tga");

	// Mid Level
	MeshBuilder::GetInstance()->GenerateOBJ("Body_Mid", "OBJ//Zombie//Mid//Body_Mid.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Head_Mid", "OBJ//Zombie//Mid//Head_Mid.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("RightArm_Mid", "OBJ//Zombie//Mid//Arm_Mid.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("LeftArm_Mid", "OBJ//Zombie//Mid//Arm_Mid.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("RightLeg_Mid", "OBJ//Zombie//Mid//Leg_Mid.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("LeftLeg_Mid", "OBJ//Zombie//Mid//Leg_Mid.obj");
	MeshBuilder::GetInstance()->GetMesh("Body_Mid")->textureID = LoadTGA("Image//Assignment1//ZombieBody_Mid.tga");
	MeshBuilder::GetInstance()->GetMesh("Head_Mid")->textureID = LoadTGA("Image//Assignment1//Zombie_Mid.tga");
	MeshBuilder::GetInstance()->GetMesh("RightArm_Mid")->textureID = LoadTGA("Image//Assignment1//ZombieArms_Mid.tga");
	MeshBuilder::GetInstance()->GetMesh("LeftArm_Mid")->textureID = LoadTGA("Image//Assignment1//ZombieArms_Mid.tga");
	MeshBuilder::GetInstance()->GetMesh("LeftLeg_Mid")->textureID = LoadTGA("Image//Assignment1//ZombieBody_Mid.tga");
	MeshBuilder::GetInstance()->GetMesh("RightLeg_Mid")->textureID = LoadTGA("Image//Assignment1//ZombieBody_Mid.tga");

	// Low Level
	MeshBuilder::GetInstance()->GenerateOBJ("Body_Lo", "OBJ//Zombie//Low//Body_Lo.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Head_Lo", "OBJ//Zombie//Low//Head_Lo.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("RightArm_Lo", "OBJ//Zombie//Low//Arm_Lo.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("LeftArm_Lo", "OBJ//Zombie//Low//Arm_Lo.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("RightLeg_Lo", "OBJ//Zombie//Low//Leg_Lo.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("LeftLeg_Lo", "OBJ//Zombie//Low//Leg_Lo.obj");
	MeshBuilder::GetInstance()->GetMesh("Body_Lo")->textureID = LoadTGA("Image//Assignment1//ZombieBody_Lo.tga");
	MeshBuilder::GetInstance()->GetMesh("Head_Lo")->textureID = LoadTGA("Image//Assignment1//Zombie_Lo.tga");
	MeshBuilder::GetInstance()->GetMesh("RightArm_Lo")->textureID = LoadTGA("Image//Assignment1//ZombieArms_Lo.tga");
	MeshBuilder::GetInstance()->GetMesh("LeftArm_Lo")->textureID = LoadTGA("Image//Assignment1//ZombieArms_Lo.tga");
	MeshBuilder::GetInstance()->GetMesh("LeftLeg_Lo")->textureID = LoadTGA("Image//Assignment1//ZombieBody_Lo.tga");
	MeshBuilder::GetInstance()->GetMesh("RightLeg_Lo")->textureID = LoadTGA("Image//Assignment1//ZombieBody_Lo.tga");

	// ------------------------------------------------- Tree
	MeshBuilder::GetInstance()->GenerateOBJ("Tree_Hi", "OBJ//tree//Tree.obj");
	MeshBuilder::GetInstance()->GetMesh("Tree_Hi")->textureID = LoadTGA("Image//Assignment1//Tree_Hi.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Tree_Med", "OBJ//tree//Tree_med.obj");
	MeshBuilder::GetInstance()->GetMesh("Tree_Med")->textureID = LoadTGA("Image//Assignment1//Tree_Med.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Tree_Lo", "OBJ//tree//Tree_lo.obj");
	MeshBuilder::GetInstance()->GetMesh("Tree_Lo")->textureID = LoadTGA("Image//Assignment1//Tree_Lo.tga");

	// ------------------------------------------------- Tombstone
	MeshBuilder::GetInstance()->GenerateOBJ("TombStone_Hi", "OBJ//tombstone//tombstone.obj");
	MeshBuilder::GetInstance()->GetMesh("TombStone_Hi")->textureID = LoadTGA("Image//Assignment1//TombStone_Hi.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("TombStone_Med", "OBJ//tombstone//tombstone_med.obj");
	MeshBuilder::GetInstance()->GetMesh("TombStone_Med")->textureID = LoadTGA("Image//Assignment1//TombStone_Med.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("TombStone_Lo", "OBJ//tombstone//tombstone_lo.obj");
	MeshBuilder::GetInstance()->GetMesh("TombStone_Lo")->textureID = LoadTGA("Image//Assignment1//TombStone_Lo.tga");

	// ------------------------------------------------- House
	MeshBuilder::GetInstance()->GenerateOBJ("House_Hi", "OBJ//house//house.obj");
	MeshBuilder::GetInstance()->GetMesh("House_Hi")->textureID = LoadTGA("Image//Assignment1//House_Hi.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("House_Med", "OBJ//house//house_med.obj");
	MeshBuilder::GetInstance()->GetMesh("House_Med")->textureID = LoadTGA("Image//Assignment1//House_Med.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("House_Lo", "OBJ//house//house_lo.obj");
	MeshBuilder::GetInstance()->GetMesh("House_Lo")->textureID = LoadTGA("Image//Assignment1//House_Lo.tga");

	// ------------------------------------------------- Turret
	MeshBuilder::GetInstance()->GenerateOBJ("Turret_Top", "OBJ//Turret//Turret_Top.obj");
	MeshBuilder::GetInstance()->GetMesh("Turret_Top")->textureID = LoadTGA("Image//Assignment1//Turret.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Turret_Top_Mid", "OBJ//Turret//Turret_Top_Mid.obj");
	MeshBuilder::GetInstance()->GetMesh("Turret_Top_Mid")->textureID = LoadTGA("Image//Assignment1//Turret_Mid.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Turret_Top_Lo", "OBJ//Turret//Turret_Top_Lo.obj");
	MeshBuilder::GetInstance()->GetMesh("Turret_Top_Lo")->textureID = LoadTGA("Image//Assignment1//Turret_Lo.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Turret_Bottom", "OBJ//Turret//Turret_Bottom.obj");
	MeshBuilder::GetInstance()->GetMesh("Turret_Bottom")->textureID = LoadTGA("Image//Assignment1//Turret.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Turret_Bottom_Mid", "OBJ//Turret//Turret_Top_Mid.obj");
	MeshBuilder::GetInstance()->GetMesh("Turret_Bottom_Mid")->textureID = LoadTGA("Image//Assignment1//Turret_Mid.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Turret_Bottom_Lo", "OBJ//Turret//Turret_Top_Lo.obj");
	MeshBuilder::GetInstance()->GetMesh("Turret_Bottom_Lo")->textureID = LoadTGA("Image//Assignment1//Turret_Lo.tga");

	// ------------------------------------------------- Spatial Partitioning ------------------------------------------------- //
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1.f, 1.f, 1.f), 10.f);
	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(40000.0f, 80000.0f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	// ------------------------------------------------- Creation of Entities ------------------------------------------------- //
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z));

	// ------------------------------------------------- File Manager ------------------------------------------------- //
	FileManager::GetInstance()->init();
	FileManager::GetInstance()->ReadMapFile("Files//Scene.csv");

	// ------------------------------------------------- Scene Graph ------------------------------------------------- //
	CEnemy* ZombieBody = Create::Enemy("Body_Hi", Vector3(0.0f, 0.f, 0.0f), Vector3(2.f, 2.f, 2.f));
	ZombieBody->Init();
	ZombieBody->InitLOD("Body_Hi", "Body_Mid", "Body_Lo");
	ZombieBody->SetAABB(Vector3(1.5f, 1.5f, 1.5f), Vector3(-1.5f, -1.5f, -1.5f));
	CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(ZombieBody);

	// Child
	// |
	// Base 
	CEnemy* ZombieHead = Create::Enemy("Head_Hi", Vector3(0.f,3.f,0.f), Vector3(2.f, 2.f, 2.f));
	ZombieHead->Init();
	ZombieHead->InitLOD("Head_Hi", "Head_Mid", "Head_Lo");
	ZombieHead->SetAABB(Vector3(2.f, 2.f, 2.f), Vector3(-2.f, -2.f, -2.f));
	CSceneNode* ZombieHeadNode = baseNode->AddChild(ZombieHead);
	//ZombieHeadNode->ApplyTranslate(0.0f, 1.0f, 0.0f);

	//// Rotate the node
	//CUpdateTransformation* aRotateMtx = new CUpdateTransformation();
	//aRotateMtx->ApplyUpdate(10.f, 0.f, 0.f, 1.f);
	//aRotateMtx->SetSteps(-120, 60);
	//ZombieHeadNode->SetUpdateTransformation(aRotateMtx);

	// Child
	// |
	// Base -> Child
	CEnemy* ZombieRightArm = Create::Enemy("RightArm_Hi", Vector3(2.5f, 0.5f, 1.f), Vector3(2.f, 2.f, 2.f));
	ZombieRightArm->Init();
	ZombieRightArm->InitLOD("RightArm_Hi", "RightArm_Mid", "RightArm_Lo");
	ZombieRightArm->SetAABB(Vector3(1.f, 1.f, 1.f), Vector3(-2.f, -1.f, -1.f));
	CSceneNode* ZombieRightArmNode = baseNode->AddChild(ZombieRightArm);
	//grandchildNode->ApplyTranslate(0.0f, 0.0f, 1.0f);

	//         Child
	//           |
	// Child <- Base -> Child
	CEnemy* ZombieLeftArm = Create::Enemy("LeftArm_Hi", Vector3(-2.5f, 0.5f, 1.f), Vector3(2.f, 2.f, 2.f));
	ZombieLeftArm->Init();
	ZombieLeftArm->InitLOD("LeftArm_Hi", "LeftArm_Mid", "LeftArm_Lo");
	ZombieLeftArm->SetAABB(Vector3(1.f, 2.f, 2.f), Vector3(-1.f, -1.f, -1.f));

	CSceneNode* ZombieLeftArmNode = baseNode->AddChild(ZombieLeftArm);

	//         Child
	//           |
	// Child <- Base -> Child
	//			|
	//		  Child
	CEnemy* ZombieRightLeg = Create::Enemy("RightLeg_Hi", Vector3(1.0f, -5.0f, 0.0f), Vector3(2.f, 5.f, 2.f));
	ZombieRightLeg->Init();
	ZombieRightLeg->InitLOD("RightLeg_Hi", "RightLeg_Mid", "RightLeg_Lo");
	ZombieRightLeg->SetAABB(Vector3(1.f, 2.5f, 1.f), Vector3(-1.f, -2.5f, -1.f));

	CSceneNode* ZombieRightLegNode = baseNode->AddChild(ZombieRightLeg);

	//         Child
	//           |
	// Child <- Base -> Child
	//		|	 	 |
	//	  Child		Child
	CEnemy* ZombieLeftLeg = Create::Enemy("LeftLeg_Hi", Vector3(-1.0f, -5.0f, 0.0f), Vector3(2.f, 5.f, 2.f));
	ZombieLeftLeg->Init();
	ZombieLeftLeg->InitLOD("LeftLeg_Hi", "LeftLeg_Mid", "LeftLeg_Lo");
	ZombieLeftLeg->SetAABB(Vector3(1.f, 2.5f, 1.f), Vector3(-1.f, -2.5f, -1.f));
	CSceneNode* ZombieLeftLegNode = baseNode->AddChild(ZombieLeftLeg);

	// ------------------------------------------------- Turret ------------------------------------------------- //


	// ------------------------------------------------- SkyBox ------------------------------------------------- //
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
		"SKYBOX_LEFT", "SKYBOX_RIGHT",
		"SKYBOX_TOP", "SKYBOX_BOTTOM");

	// ------------------------------------------------- Ground ------------------------------------------------- //
	groundEntity = Create::Ground("SKYBOX_BOTTOM", "SKYBOX_BOTTOM");
	
	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	//groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	playerInfo->SetTerrain(groundEntity);
	//theEnemy->SetTerrain(groundEntity);

	//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	// ------------------------------------------------- Enemy ------------------------------------------------- //
	//theEnemy = new CEnemy();
	//theEnemy->Init();

	// ------------------------------------------------- 2D Entities ------------------------------------------------- //
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 10; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}
	textObj[0]->SetText("HELLO WORLD");

	// Highscore
	m_dTimer = 0.0;
	m_iCurrScore = CLuaInterface::GetInstance()->getIntValue("CurrScore", 1);
	m_iHighScore = CLuaInterface::GetInstance()->getIntValue("Highscore", 1);
}

void SceneText::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);
	
	/*Map Editor*/
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD2))
		MapEditor::GetInstance()->mapEditing = true;

	if (MapEditor::GetInstance()->mapEditing)
		MapEditor::GetInstance()->updateOption(dt);

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		FileManager::GetInstance()->EditMapFile("Files//Scene.csv");

	if (KeyboardController::GetInstance()->IsKeyPressed('0'))
		CSpatialPartition::GetInstance()->renderPos = true;

	if (KeyboardController::GetInstance()->IsKeyPressed('9'))
		CSpatialPartition::GetInstance()->renderPos = false;

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if (KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if (KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
	// <THERE>

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)
	// Updates the light 
	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Updates the Highscore / CurrScore
	m_dTimer += static_cast<float>(dt);
	if (m_dTimer > 10.f)
	{
		// Add 10pts every 10s stay alive/ remain in game
		m_iCurrScore += 10;
		CLuaInterface::GetInstance()->saveIntValue("CurrScore", m_iCurrScore, 1, true);

		// Highscore - When higher then previous highscore, replace the values
		if (m_iHighScore < m_iCurrScore)
			CLuaInterface::GetInstance()->saveIntValue("Highscore", m_iCurrScore, 1);
		else
			CLuaInterface::GetInstance()->saveIntValue("Highscore", m_iHighScore, 1);

		m_dTimer = 0.f;
	}

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	textObj[1]->SetText(ss.str());

	ss.str("");
	ss.precision(4);
	ss << "Player:" << playerInfo->GetPos();
	textObj[2]->SetText(ss.str());

	ss.str("");
	ss.precision(2);
	ss << "Entities in Grid: " << CSpatialPartition::GetInstance()->GetObjectsSize();
	textObj[3]->SetText(ss.str());

	// Highscore
	ss.str("");
	ss << "Current Score: " << m_iCurrScore;
	textObj[4]->SetText(ss.str());

	ss.str("");
	if (m_iHighScore > m_iCurrScore)
		ss << "HighScore: " << m_iHighScore;
	else
		ss << "HighScore: " << m_iCurrScore;

	textObj[5]->SetText(ss.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();

	if (MapEditor::GetInstance()->mapEditing)
		MapEditor::GetInstance()->renderObj();

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();

	/*Render Map Editor Options.*/
	if (MapEditor::GetInstance()->mapEditing)
		MapEditor::GetInstance()->renderOption();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];

	// Reset Highscore
	//CLuaInterface::GetInstance()->saveIntValue("CurrScore", m_iCurrScore, 1, true);
}