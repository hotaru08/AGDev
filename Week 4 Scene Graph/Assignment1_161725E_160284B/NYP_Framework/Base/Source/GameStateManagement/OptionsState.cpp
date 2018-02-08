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
#include <sstream>
#include <string>

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
	MeshBuilder::GetInstance()->GetMesh("OptionSTATE_BKGROUND")->textureID = LoadTGA("Image//OptionsState.tga");

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	OptionStateBackground = Create::Sprite2DObject("OptionSTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(800.0f, 600.0f, 0.0f));

	MeshBuilder::GetInstance()->GenerateText("text2", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text2")->textureID = LoadTGA("Image//calibri.tga");
	//MeshBuilder::GetInstance()->GetMesh("text2")->material.kAmbient.Set(1, 0, 0);
	cout << "COptionState loaded\n" << endl;

	NumCase = 0;
	adjustReso = 0;
	adjustMouse = 50;

	MeshBuilder::GetInstance()->GenerateQuad("bRed", Color(0.9, 0, 0), 1.f);
}

void COptionState::Update(double dt)
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
		NumCase = 2;
	}
	else if (NumCase > 2)
	{
		NumCase = 0;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('A'))
	{
		switch (NumCase)
		{
		case 0:
			adjustReso -= 1;
			break;
		case 1:
			adjustMouse -= 10;
			break;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('D'))
	{
		switch (NumCase)
		{
		case 0:
			adjustReso += 1;
			break;
		case 1:
			adjustMouse += 10;
			break;
		}
	}
	
	if (adjustReso > 2)
		adjustReso = 0;
	else if (adjustReso < 0)
		adjustReso = 2;

	if (adjustMouse > 100)
		adjustMouse = 100;
	else if (adjustMouse < 0)
		adjustMouse = 0;

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) && NumCase == 2)
	{
		cout << "Loading MenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
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

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	if(NumCase < 2)
		modelStack.Translate(Application::GetInstance().GetWindowWidth() * 0.5 - 180, Application::GetInstance().GetWindowHeight() * 0.5 - (NumCase * 110) + 70, 10);
	else
		modelStack.Translate(Application::GetInstance().GetWindowWidth() * 0.5 + 160, Application::GetInstance().GetWindowHeight() * 0.5 - 190, 10);

	modelStack.Scale(50, 7.5, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Red"));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(Application::GetInstance().GetWindowWidth() * 0.5 + (adjustReso * 100) + 110, Application::GetInstance().GetWindowHeight() * 0.5 + 135, 10);
	modelStack.Scale(10, 10, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("bRed"));
	modelStack.PopMatrix();

	string mouse = to_string(adjustMouse);

	modelStack.PushMatrix();
	modelStack.Translate(Application::GetInstance().GetWindowWidth() * 0.5f + 150, Application::GetInstance().GetWindowHeight() * 0.5f - 50, 1.f);
	modelStack.Scale(50, 50, 1);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text2"), mouse, Color(0.5f, 0.f, 0.f));
	modelStack.PopMatrix();
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
