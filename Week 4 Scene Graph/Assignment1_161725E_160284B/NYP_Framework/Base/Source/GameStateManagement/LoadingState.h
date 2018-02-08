#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

class SceneManager;
class CLoadingState : public Scene
{
	FPSCamera camera;
	SpriteEntity* LoadingStateBackground;
	float timer;
	float moveUp;

public:
	CLoadingState();
	~CLoadingState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};