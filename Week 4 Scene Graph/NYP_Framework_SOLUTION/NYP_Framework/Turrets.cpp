#include "Turrets.h"

#include "../../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"

#include "../../MoneyManager/Money.h"

Turret::Turret(Mesh * _modelMesh) 
	: defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, rotate(0.f, 0.f, 0.f)
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)

	, m_bFire(false)
	, m_dCoolDown(0.0)
{
	this->modelMesh = _modelMesh;
}

Turret::~Turret()
{
}

void Turret::Init(void)
{
}

void Turret::SetFire(bool fire)
{
	m_bFire = fire;
}

bool Turret::GetFire()
{
	return m_bFire;
}

void Turret::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	// if the y position is not equal to terrain height at that position, 
	// then update y position to the terrain height
	if (position.y <= m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

void Turret::SetMesh(Mesh * mesh)
{
	modelMesh = mesh;
}

void Turret::SetPos(const Vector3 & pos)
{
	this->position = pos;
}

Vector3 Turret::GetPos(void) const
{
	return position;
}

void Turret::SetTarget(const Vector3 & target)
{
	this->target = target;
}

Vector3 Turret::GetTarget(void) const
{
	return target;
}

void Turret::SetUp(const Vector3 & up)
{
	this->up = up;
}

Vector3 Turret::GetUp(void) const
{
	return up;
}

void Turret::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

void Turret::SetTerrain(GroundEntity * m_pTerrain)
{
	this->m_pTerrain = m_pTerrain;
}

GroundEntity * Turret::GetTerrain(void)
{
	return m_pTerrain;
}

void Turret::SetType(int type)
{
	switch (type)
	{
	case 1: //normal turret
		this->type = TURRET_TYPE::TURRET;
		break;
	case 2: //rapid fire
		this->type = TURRET_TYPE::RAPIDFIRE_TURRET;
		break;
	case 3: //wide range
		this->type = TURRET_TYPE::WIDERANGE_TURRET;
		break;
	default:
		break;
	}
}

int Turret::GetType()
{
	return type;
}

void Turret::Update(double dt)
{
	//Normal turret
	if (m_bFire && TURRET_TYPE::TURRET)
	{
		rotate = (fireDestination - position).Normalized();
		Vector3 newPos = position;
		newPos.x += rotate.x * 20.f;
		newPos.z += rotate.z * 20.f;
		newPos.y += 17.f;
		CProjectile* aProjectile = Create::Projectile("Turretbullet", newPos, (fireDestination - newPos).Normalized(), 4.0f, 100.0f, 2, 1, this);
		aProjectile->SetCollider(true);
		aProjectile->SetFireDestination(fireDestination);
		m_bFire = false;
	}

	// Rapid fire
	else if (m_bFire && TURRET_TYPE::RAPIDFIRE_TURRET)
	{
		rotate = (fireDestination - position).Normalized();
		Vector3 newPos = position;
		newPos.x += rotate.x * 20.f;
		newPos.z += rotate.z * 20.f;
		newPos.y += 17.f;
		CProjectile* aProjectile = Create::Projectile("Turretbullet", newPos, (fireDestination - newPos).Normalized(), 4.0f, 100.0f, 2, 1, this);
		aProjectile->SetCollider(true);
		aProjectile->SetFireDestination(fireDestination);
		m_bFire = false;
	}

	// Wide range
	else if (m_bFire && TURRET_TYPE::WIDERANGE_TURRET)
	{
		rotate = (fireDestination - position).Normalized();
		Vector3 newPos = position;
		newPos.x += rotate.x * 20.f;
		newPos.z += rotate.z * 20.f;
		newPos.y += 17.f;
		CProjectile* aProjectile = Create::Projectile("Turretbullet", newPos, (fireDestination - newPos).Normalized(), 4.0f, 200.0f, 2, 3, this);
		aProjectile->SetCollider(true);
		aProjectile->SetFireDestination(fireDestination);
		m_bFire = false;
	}
}

void Turret::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();

	// Turret
	if (TURRET_TYPE::TURRET)
	{
		// Bottom of turret
		modelStack.Translate(position.x, position.y, position.z);

		// Top of turret
		modelStack.PushMatrix();
		modelStack.Rotate(Math::RadianToDegree(atan2(rotate.x, rotate.z)), 0, 1, 0);
		modelStack.Translate(0, 0, -1);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Rotate(-20, 0, 0, 1);
		modelStack.Scale(scale.x, scale.y, scale.z);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();

		modelStack.Scale(scale.x, scale.y, scale.z);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("turretBot"));
	}

	// Rapid Fire
	else if (TURRET_TYPE::RAPIDFIRE_TURRET)
	{
		// Bottom of turret
		modelStack.Translate(position.x, position.y, position.z);

		// Top of turret
		modelStack.PushMatrix();
		modelStack.Rotate(Math::RadianToDegree(atan2(rotate.x, rotate.z)), 0, 1, 0);
		modelStack.Translate(0, 0, -1);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Rotate(-20, 0, 0, 1);
		modelStack.Scale(scale.x, scale.y, scale.z);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();

		modelStack.Scale(scale.x, scale.y, scale.z);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("rturretBot"));
	}

	//wide fire
	else
	{
		// Bottom of turret
		modelStack.Translate(position.x, position.y, position.z);

		// Top of turret
		modelStack.PushMatrix();
		modelStack.Rotate(Math::RadianToDegree(atan2(rotate.x, rotate.z)), 0, 1, 0);
		modelStack.Translate(0, 0, -1);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Rotate(-20, 0, 0, 1);
		modelStack.Scale(scale.x, scale.y, scale.z);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();

		modelStack.Scale(scale.x, scale.y, scale.z);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("wturretBot"));
		
	}

	modelStack.PopMatrix();

	//Health
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y + scale.y * 2 + 20, position.z);
	modelStack.Scale(m_iHealth / 5.f, 1.f, 1.f);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("healthR"));
	modelStack.PopMatrix();
}

Turret * Create::Turret3D(const std::string & _meshName, 
	const Vector3 & _position, 
	const Vector3 & _scale, 
	const int type)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Turret* result = new Turret(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(true);
	result->SetType(type);
	result->SetMeshName(_meshName);

	switch (type)
	{
	case 1:
		result->SetHealth(100);
		EntityManager::GetInstance()->AddTurretEntity(result);
		break;
	case 2:
		result->SetHealth(100);
		EntityManager::GetInstance()->AddTurretEntity(result);
		break;
	case 3:
		result->SetHealth(200);
		EntityManager::GetInstance()->AddTurretEntity(result);
		break;
	default:
		break;
	}
	return result;
}