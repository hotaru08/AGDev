#include "OptionsState.h"

#include <iostream>
using namespace std;

#include "GL\glew.h"
#include "GLFW\glfw3.h"
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
#include "../lua/CLuaInterface.h"
#include <sstream>
#include <string>

extern GLFWwindow* m_window;
COptionState::COptionState()
{
}

COptionState::~COptionState()
{
}

void COptionState::Init()
{
	// Clear entities and initialise again
	EntityManager::GetInstance()->cleartheList();
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Getting Values
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;

	m_width = Application::GetInstance().GetWindowWidth();
	m_height = Application::GetInstance().GetWindowHeight();

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("OptionSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("OptionSTATE_BKGROUND")->textureID = LoadTGA("Image//OptionsState.tga");
	OptionStateBackground = Create::Sprite2DObject("OptionSTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(m_width, m_height, 0.0f));

	MeshBuilder::GetInstance()->GenerateText("text2", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text2")->textureID = LoadTGA("Image//calibri.tga");
	//MeshBuilder::GetInstance()->GetMesh("text2")->material.kAmbient.Set(1, 0, 0);
	cout << "COptionState loaded\n" << endl;

	// Variables
	NumCase = 0;
	adjustReso = 0;
	adjustMouse = 50;

	//m_height = CLuaInterface::GetInstance()->getIntValue("height", 2);
	//m_width = CLuaInterface::GetInstance()->getIntValue("width", 2);

	// Arrow to show which choice player is at
	MeshBuilder::GetInstance()->GenerateQuad("bRed", Color(0.9, 0, 0), 1.f);

	//// ------------------------------------------------- 2D Entities ------------------------------------------------- //
	//float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	//float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	//float fontSize = 25.0f;
	//float halfFontSize = fontSize / 2.0f;
	//for (int i = 0; i < 10; ++i)
	//{
	//	textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	//}
	//textObj[0]->SetText("HELLO WORLD");
}

void COptionState::Update(double dt)
{
	// Selction of options
	if (KeyboardController::GetInstance()->IsKeyPressed('W'))
	{
		NumCase--;
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('S'))
	{
		NumCase++;
	}

	// Go to the max / min options when overshot
	if (NumCase < 0)
	{
		NumCase = 2;
	}
	else if (NumCase > 2)
	{
		NumCase = 0;
	}

	// for the left right options choosing
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
	
	// Wrapping if overshot
	if (adjustReso > 2)
		adjustReso = 0;
	else if (adjustReso < 0)
		adjustReso = 2;

	// Limiting Values of mouse sensitivity
	if (adjustMouse > 100)
		adjustMouse = 100;
	else if (adjustMouse < 0)
		adjustMouse = 0;

	// Lua : Changing Screen resolutions ( 0, 1, 2)
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
	{
		switch (adjustReso)
		{
		case 0: // default screen 800 x 600
			CLuaInterface::GetInstance()->saveIntValue("width", 800, 2, true);
			CLuaInterface::GetInstance()->saveIntValue("height", 600, 2);
			m_width = 800;
			m_height = 600;
			break;
		case 1: // middle screen 1600 x 900
			CLuaInterface::GetInstance()->saveIntValue("width", 1600, 2, true);
			CLuaInterface::GetInstance()->saveIntValue("height", 900, 2);
			m_width = 1600;
			m_height = 900;
			break;
		case 2: // full screen 1920 x 1080
			CLuaInterface::GetInstance()->saveIntValue("width", 1920, 2, true);
			CLuaInterface::GetInstance()->saveIntValue("height", 1080, 2);
			m_width = 1920;
			m_height = 1080;
			break;
		}	

		glfwSetWindowSize(m_window, m_width, m_height);
	}
	

	// Return back to main menu
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) && NumCase == 2)
	{
		cout << "Loading MenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	/*std::ostringstream ss;
	ss.precision(2);
	ss << "Entities in Grid: " << CSpatialPartition::GetInstance()->GetObjectsSize();
	textObj[3]->SetText(ss.str());*/
}

void COptionState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// setup 3D pipeline then render
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
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

	// Highlight to choice
	modelStack.PushMatrix();
	if(NumCase < 2)
		modelStack.Translate(m_width * 0.5 - 180, m_height * 0.5 - (NumCase * 110) + 70, 10);
	else
		modelStack.Translate(m_width * 0.5 + 160, m_height * 0.5 - 190, 10);

	modelStack.Scale(50, 7.5, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Red"));
	modelStack.PopMatrix();

	// Small red block
	modelStack.PushMatrix();
	modelStack.Translate(m_width * 0.5 + (adjustReso * 100) + 110, m_height * 0.5 + 135, 10);
	modelStack.Scale(10, 10, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("bRed"));
	modelStack.PopMatrix();

	string mouse = to_string(adjustMouse);

	// Mouse Sensitivity
	modelStack.PushMatrix();
	modelStack.Translate(m_width * 0.75f, m_height * 0.5f, 1.f);
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
