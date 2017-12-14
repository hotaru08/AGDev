#include "MapEditor.h"
#include "MeshBuilder.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Application.h"
#include <string>
#include "KeyboardController.h"
#include "../GenericEntity.h"
#include "../FileManager/FileManager.h"

using std::string;

MapEditor::MapEditor() :
	objType(OBJECT)
	, displacement(0.f, -10.f, 0.f)
	, displacementMod(0.f, -10.f, 0.f)
	, scale(1.f, 1.f, 1.f)
	, rotate(0.f)
	, ObjectType_String("")
	, ScaleAxis_String("")
	, scaleAxis(SCALE_ALL)
	, mapEditing(false)
{
}

MapEditor::~MapEditor()
{
}

void MapEditor::renderObj(void)
{
	if (mapEditing)
	{
		switch (objType)
		{
		case HOUSE:
		{
			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(displacement.x, displacement.y, displacement.z);
			modelStack.Rotate(rotate, 0.f, 1.f, 0.f);
			modelStack.Scale(scale.x, scale.y, scale.z);
			RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("House_Hi"));
			modelStack.PopMatrix();
			break;
		}

		case TREE:
		{
			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(displacement.x, displacement.y, displacement.z);
			modelStack.Rotate(rotate, 0.f, 1.f, 0.f);
			modelStack.Scale(scale.x, scale.y, scale.z);
			RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("Tree_Hi"));
			modelStack.PopMatrix();
			break;
		}

		case TOMBSTONE:
		{
			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(displacement.x, displacement.y, displacement.z);
			modelStack.Rotate(rotate, 0.f, 1.f, 0.f);
			modelStack.Scale(scale.x, scale.y, scale.z);
			RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("TombStone_Hi"));
			modelStack.PopMatrix();
			break;
		}
		}
	}
}

void MapEditor::renderOption(void)
{
	//Calculating aspect ratio
	float windowHeight = Application::GetInstance().GetWindowHeight();
	float windowWidth = Application::GetInstance().GetWindowWidth();

	static string ObjectS;
	static string scaleAxisS;

	switch (objType)
	{
	case OBJECT:
	{
		ObjectS = "NONE";
		break;
	}

	case HOUSE:
	{
		ObjectS = "HOUSE";
		break;

	}

	case TREE:
	{
		ObjectS = "TREE";
		break;

	}

	case TOMBSTONE:
	{
		ObjectS = "TOMBSTONE";
		break;

	}
	}

	switch (scaleAxis)
	{
	case SCALE_ALL:
	{
		scaleAxisS = "ALL";
		break;

	}

	case SCALE_X:
	{
		scaleAxisS = "X";
		break;

	}

	case SCALE_Y:
	{
		scaleAxisS = "Y";
		break;

	}

	case SCALE_Z:
	{
		scaleAxisS = "Z";
		break;

	}
	}

	ObjectType_String.str(ObjectS);
	ScaleAxis_String.str(scaleAxisS);

	std::ostringstream playerPosition("");
	playerPosition << CPlayerInfo::GetInstance()->GetPos();

	std::ostringstream playerTarget("");
	playerTarget << CPlayerInfo::GetInstance()->GetTarget();

	std::ostringstream ss_ObjectRotate("");
	ss_ObjectRotate << rotate;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(-windowWidth * 0.48f, windowHeight * 0.25f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.03f, Application::GetInstance().GetWindowWidth() * 0.03f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Object Type:" + ObjectType_String.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-windowWidth * 0.48f, windowHeight * 0.20f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.03f, Application::GetInstance().GetWindowWidth() * 0.03f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Scale Axis:" + ScaleAxis_String.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-windowWidth * 0.48f, windowHeight * 0.15f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.03f, Application::GetInstance().GetWindowWidth() * 0.03f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Rotate:" + ss_ObjectRotate.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-windowWidth * 0.48f, windowHeight * 0.10f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.03f, Application::GetInstance().GetWindowWidth() * 0.03f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Player Position:" + playerPosition.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-windowWidth * 0.48f, windowHeight * 0.05f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.03f, Application::GetInstance().GetWindowWidth() * 0.03f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Player Target:" + playerTarget.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();
}

void MapEditor::updateOption(double dt)
{
	static CPlayerInfo* _player = CPlayerInfo::GetInstance();

	displacement.Set(_player->GetTarget().x - _player->GetPos().x, -10.f, _player->GetTarget().z - _player->GetPos().z);
	displacement.Set(static_cast<float>((displacement.x * 50.f) + _player->GetPos().x), -10.f, static_cast<float>((displacement.z * 50.f) + _player->GetPos().z));

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD4))
	{
		if (objType == OBJECT)
			objType = HOUSE;

		else if (objType == HOUSE)
			objType = TREE;

		else if (objType == TREE)
			objType = TOMBSTONE;

		else if (objType == TOMBSTONE)
			objType = OBJECT;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD6))
	{
		if (objType == OBJECT)
			objType = TOMBSTONE;

		else if (objType == HOUSE)
			objType = OBJECT;

		else if (objType == TREE)
			objType = HOUSE;

		else if (objType == TOMBSTONE)
			objType = TREE;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD9))
	{
		mapEditing = false;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD8))
	{
		if (rotate >= 270.f)
			rotate = 0.f;
		else if (rotate < 270)
			rotate += 90;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD3))
	{
		if (scaleAxis == SCALE_X)
			scale.x += 1.f;

		else if (scaleAxis == SCALE_Y)
			scale.y += 1.f;

		else if (scaleAxis == SCALE_Z)
			scale.z += 1.f;

		else if (scaleAxis == SCALE_ALL)
		{
			scale.x += 1.f;
			scale.y += 1.f;
			scale.z += 1.f;
		}

		if (scale.x > 5 && scale.y > 5 && scale.z > 5)
		{
			scale.x = 5.0f;
			scale.y = 5.0f;
			scale.z = 5.0f;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD1))
	{
		if (scaleAxis == SCALE_X)
			scale.x -= 1.f;

		else if (scaleAxis == SCALE_Y)
			scale.y -= 1.f;

		else if (scaleAxis == SCALE_Z)
			scale.z -= 1.f;

		else if (scaleAxis == SCALE_ALL)
		{
			scale.x -= 1.f;
			scale.y -= 1.f;
			scale.z -= 1.f;
		}

		if (scale.x < 1 && scale.y < 1 && scale.z < 1)
		{
			scale.x = 1.0f;
			scale.y = 1.0f;
			scale.z = 1.0f;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD5))
	{
		switch (objType)
		{
		case HOUSE:
		{
			GenericEntity* House = Create::Entity("House_Hi", displacement, scale);
			House->SetCollider(true);
			House->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			House->InitLOD("House_Hi", "House_Med", "House_Lo");
			break;
		}

		case TREE:
		{
			GenericEntity* Tree = Create::Entity("Tree_Hi", displacement, scale);
			Tree->SetCollider(true);
			Tree->SetRotate(rotate);
			Tree->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			Tree->InitLOD("Tree_Hi", "Tree_Med", "Tree_Lo");
			break;
		}

		case TOMBSTONE:
		{
			GenericEntity* Tomb = Create::Entity("TombStone_Hi", displacement, scale);
			Tomb->SetCollider(true);
			Tomb->SetRotate(rotate);
			Tomb->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			Tomb->InitLOD("TombStone_Hi", "TombStone_Med", "TombStone_Lo");
			break;
		}
		}
	}
}
