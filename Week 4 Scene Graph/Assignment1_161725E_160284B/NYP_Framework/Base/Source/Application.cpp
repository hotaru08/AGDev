#include "Application.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneText.h"
#include "GameStateManagement\IntroState.h"
#include "GameStateManagement\MenuState.h"
#include "GameStateManagement\LoadingState.h"
#include "GameStateManagement\OptionsState.h"

#include "lua\CLuaInterface.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application() : 
	m_window_width(640), m_window_height(480)
{
}

Application::~Application()
{
}

void Application::Init()
{
	// init the lua system
	CLuaInterface::GetInstance()->Init();

	// get the open gl resolution
	m_window_width = CLuaInterface::GetInstance()->getIntValue("width", 2);
	m_window_height = CLuaInterface::GetInstance()->getIntValue("height", 2);

	//CLuaInterface::GetInstance()->Run();
	CLuaInterface::GetInstance()->saveFloatValue("Player1", 200.10, 1, true);
	CLuaInterface::GetInstance()->saveIntValue("Player2", 100, 1);

	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, "NYP Framework", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetMouseButtonCallback(m_window, &Application::MouseButtonCallbacks);
	glfwSetScrollCallback(m_window, &Application::MouseScrollCallbacks);

	// Init systems
	GraphicsManager::GetInstance()->Init();

	// Week 13 - Game State Management ( Add States to SceneManager )
	// Create the Game States
	SceneManager::GetInstance()->AddScene("IntroState", new CIntroState());
	SceneManager::GetInstance()->AddScene("MenuState", new CMenuState());
	SceneManager::GetInstance()->AddScene("LoadingState", new CLoadingState());
	SceneManager::GetInstance()->AddScene("OptionState", new COptionState());
	SceneManager::GetInstance()->AddScene("GameState", new SceneText());

	// Set the active scene
	SceneManager::GetInstance()->SetActiveScene("IntroState");
}

void Application::Run()
{
	// Initialise display before running loop
	InitDisplay();

	//SceneManager::GetInstance()->SetActiveScene("Start");
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		glfwPollEvents();
		UpdateInput();
		
		SceneManager::GetInstance()->Update(m_timer.getElapsedTime());
		SceneManager::GetInstance()->Render();

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...

        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
		
		PostInputUpdate();
	}
	SceneManager::GetInstance()->Exit();
}

void Application::Exit()
{
	//drop the lua system
	CLuaInterface::GetInstance()->Drop();
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

void Application::UpdateInput()
{
	// Update Mouse Position
	double mouse_currX, mouse_currY;
	glfwGetCursorPos(m_window, &mouse_currX, &mouse_currY);
	MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);

	// Update Keyboard Input
	for (int i = 0; i < KeyboardController::MAX_KEYS; ++i)
		KeyboardController::GetInstance()->UpdateKeyboardStatus(i, IsKeyPressed(i));
}

void Application::PostInputUpdate()
{
	// If mouse is centered, need to update the center position for next frame
	if (MouseController::GetInstance()->GetKeepMouseCentered())
	{
		double mouse_currX, mouse_currY;
		mouse_currX = m_window_width >> 1;
		mouse_currY = m_window_height >> 1;
		MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);
		glfwSetCursorPos(m_window, mouse_currX, mouse_currY);
	}

	// Call input systems to update at end of frame
	MouseController::GetInstance()->EndFrameUpdate();
	KeyboardController::GetInstance()->EndFrameUpdate();
}

void Application::MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods)
{
	// Send the callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		MouseController::GetInstance()->UpdateMouseButtonPressed(button);
	else
		MouseController::GetInstance()->UpdateMouseButtonReleased(button);
}

void Application::MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset)
{
	MouseController::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}

int Application::GetWindowHeight()
{
	return m_window_height;
}

int Application::GetWindowWidth()
{
	return m_window_width;
}

// Initialise OpenGL environment
void Application::InitDisplay(void)
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");

	// Tell the shader program to store these uniform locations
	{
		currProg->AddUniform("MVP");
		currProg->AddUniform("MV");
		currProg->AddUniform("MV_inverse_transpose");
		currProg->AddUniform("material.kAmbient");
		currProg->AddUniform("material.kDiffuse");
		currProg->AddUniform("material.kSpecular");
		currProg->AddUniform("material.kShininess");
		currProg->AddUniform("lightEnabled");
		currProg->AddUniform("numLights");
		currProg->AddUniform("lights[0].type");
		currProg->AddUniform("lights[0].position_cameraspace");
		currProg->AddUniform("lights[0].color");
		currProg->AddUniform("lights[0].power");
		currProg->AddUniform("lights[0].kC");
		currProg->AddUniform("lights[0].kL");
		currProg->AddUniform("lights[0].kQ");
		currProg->AddUniform("lights[0].spotDirection");
		currProg->AddUniform("lights[0].cosCutoff");
		currProg->AddUniform("lights[0].cosInner");
		currProg->AddUniform("lights[0].exponent");
		currProg->AddUniform("lights[1].type");
		currProg->AddUniform("lights[1].position_cameraspace");
		currProg->AddUniform("lights[1].color");
		currProg->AddUniform("lights[1].power");
		currProg->AddUniform("lights[1].kC");
		currProg->AddUniform("lights[1].kL");
		currProg->AddUniform("lights[1].kQ");
		currProg->AddUniform("lights[1].spotDirection");
		currProg->AddUniform("lights[1].cosCutoff");
		currProg->AddUniform("lights[1].cosInner");
		currProg->AddUniform("lights[1].exponent");
		currProg->AddUniform("colorTextureEnabled");
		currProg->AddUniform("colorTexture");
		currProg->AddUniform("textEnabled");
		currProg->AddUniform("textColor");
	}

	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	currProg->UpdateInt("numLights", 0);
	currProg->UpdateInt("textEnabled", 0);
}
