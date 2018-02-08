#include "OptionsState.h"

#include <iostream>
using namespace std;

#include "GL\glew.h"
#include "../Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../TextEntity.h"
#include "RenderHelper.h"
#include "../SpriteEntity.h"
#include "../EntityManager.h"
#include "KeyboardController.h"
#include "SceneManager.h"

COptionState::COptionState()
{
}

COptionState::~COptionState()
{
}

void COptionState::Init()
{
	EntityManager::GetInstance()->cleartheList();
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("OptionSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("OptionSTATE_BKGROUND")->textureID = LoadTGA("Image//OptionState.tga");

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	OptionStateBackground = Create::Sprite2DObject("OptionSTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(800.0f, 600.0f, 0.0f));

	cout << "COptionState loaded\n" << endl;
}

void COptionState::Update(double dt)
{
	// Load GameState when spacebar is pressed
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
	{
		cout << "Loading GameState" << endl;
		SceneManager::GetInstance()->SetActiveScene("GameState");
	}
}

void COptionState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// setup 3D pipeline then render
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	//Render the required entities
	EntityManager::GetInstance()->Render();

	// setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0,
		Application::GetInstance().GetWindowWidth(),
		0,
		Application::GetInstance().GetWindowHeight(),
		-10, 10);

	GraphicsManager::GetInstance()->DetachCamera();

	// render the required entities
	EntityManager::GetInstance()->RenderUI();
}

void COptionState::Exit()
{
	// remove the entity from entitymanager 
	EntityManager::GetInstance()->RemoveEntity(OptionStateBackground);

	// remove the meshes which are specific to COptionState
	MeshBuilder::GetInstance()->RemoveMesh("OptionSTATE_BKGROUND");

	// detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}
