#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

class SceneManager;
class CMenuState : public Scene
{
	FPSCamera camera;
	SpriteEntity* MenuStateBackground;

public:
	CMenuState();
	~CMenuState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};