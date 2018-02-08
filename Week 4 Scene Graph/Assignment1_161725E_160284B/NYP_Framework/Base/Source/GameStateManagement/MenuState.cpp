#include "MenuState.h"

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

CMenuState::CMenuState()
{
}

CMenuState::~CMenuState()
{
}

void CMenuState::Init()
{
	EntityManager::GetInstance()->cleartheList();
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("Red", Color(0.3, 0, 0), 1.f);

	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//MenuState.tga");

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	MenuStateBackground = Create::Sprite2DObject("MENUSTATE_BKGROUND",
													Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
													Vector3(800.0f, 600.0f, 0.0f));

	NumCase = 0;

	cout << "CMenuState loaded\n" << endl;
}

void CMenuState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyPressed('W'))
	{
		NumCase -= 1;
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('S'))
	{
		NumCase += 1;
	}

	if (NumCase < 0)
	{
		NumCase = 1;
	}
	else if (NumCase > 1)
	{
		NumCase = 0;
	}

	// Load GameState when spacebar is pressed
	if (NumCase == 0 && KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
	{
		cout << "Loading LoadingState" << endl;
		SceneManager::GetInstance()->SetActiveScene("LoadingState");
	}
	else if (NumCase == 1 && KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
	{
		cout << "Loading OptionsState" << endl;
		SceneManager::GetInstance()->SetActiveScene("OptionState");
	}
}

void CMenuState::Render()
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

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(Application::GetInstance().GetWindowWidth() * 0.5 - 30, Application::GetInstance().GetWindowHeight() * 0.5 - (NumCase * 70), 10);
	modelStack.Scale(50, 7.5, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Red"));
	modelStack.PopMatrix();
}

void CMenuState::Exit()
{
	// remove the entity from entitymanager 
	EntityManager::GetInstance()->RemoveEntity(MenuStateBackground);

	// remove the meshes which are specific to CMenuState
	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BKGROUND");

	// detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}
