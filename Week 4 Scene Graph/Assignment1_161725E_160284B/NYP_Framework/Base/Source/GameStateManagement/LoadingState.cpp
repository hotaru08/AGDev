#include "LoadingState.h"

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

CLoadingState::CLoadingState()
{
}

CLoadingState::~CLoadingState()
{
}

void CLoadingState::Init()
{
	EntityManager::GetInstance()->cleartheList();
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("Black", Color(0, 0, 0), 1.f);

	MeshBuilder::GetInstance()->GenerateQuad("LOADINGSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("LOADINGSTATE_BKGROUND")->textureID = LoadTGA("Image//LoadingState.tga");

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	LoadingStateBackground = Create::Sprite2DObject("LOADINGSTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(800.0f, 600.0f, 0.0f));

	cout << "CLoadingState loaded\n" << endl;

	timer = 0.f;
}

void CLoadingState::Update(double dt)
{
	// Load GameState when spacebar is pressed
	timer += dt;
	if (timer >= 5)
	{
		cout << "Loading GameState" << endl;
		SceneManager::GetInstance()->SetActiveScene("GameState");
	}
}

void CLoadingState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// setup 3D pipeline then render
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	

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

	modelStack.PushMatrix();
	modelStack.Translate(Application::GetInstance().GetWindowWidth() * 0.5, (Application::GetInstance().GetWindowHeight() * 0.5) + timer * 120, 10);
	modelStack.Scale(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Black"));
	modelStack.PopMatrix();
}

void CLoadingState::Exit()
{
	// remove the entity from entitymanager 
	EntityManager::GetInstance()->RemoveEntity(LoadingStateBackground);

	// remove the meshes which are specific to CLoadingState
	MeshBuilder::GetInstance()->RemoveMesh("LoadingSTATE_BKGROUND");

	// detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}
