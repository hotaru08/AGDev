#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../TextEntity.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

class SceneManager;
class COptionState : public Scene
{
	FPSCamera camera;
	SpriteEntity* OptionStateBackground;
	int NumCase;
	int adjustReso;
	int adjustMouse;
	TextEntity* textObj[10];

	// Height and Width of screen
	int m_height, m_width;

public:
	COptionState();
	~COptionState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};