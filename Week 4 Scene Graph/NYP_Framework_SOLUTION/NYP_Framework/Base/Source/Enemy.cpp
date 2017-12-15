#include "Enemy.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "PlayerInfo\PlayerInfo.h"

CEnemy::CEnemy(Mesh* modelMesh)
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, health(0)
	, m_pTerrain(NULL)
{
	this->modelMesh = modelMesh;
}

CEnemy::~CEnemy()
{
	m_pTerrain = NULL;
}

void CEnemy::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);
	
	// Set the current values
	//position.Set(10.0f, 0.0f, 0.0f);
	target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);
	
	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);
	
	// Set speed
	m_dSpeed = 1.0;

	// Set health
	health = 1;

	// Initialise the LOD meshes
	//InitLOD("Body_Hi", "Body_Hi", "Body_Hi");

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));
}

void CEnemy::Reset(void)
{
	// set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

void CEnemy::SetPos(const Vector3 & pos)
{
	position = pos; 
}

void CEnemy::SetTarget(const Vector3 & target)
{
	this->target = target;
}

void CEnemy::SetUp(const Vector3 & up)
{
	this->up = up;
}

void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

void CEnemy::SetTerrain(GroundEntity * m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Set the health of enemies
void CEnemy::SetHealth(int health)
{
	this->health = health;
}

// Get the health of enemies
int CEnemy::GetHealth()
{
	return health;
}

Vector3 CEnemy::GetPos(void) const
{
	return this->position;
}

Vector3 CEnemy::GetTarget(void) const
{
	return this->target;
}

Vector3 CEnemy::GetUp(void) const
{
	return this->up;
}

GroundEntity * CEnemy::GetTerrain(void)
{
	return this->m_pTerrain;
}

void CEnemy::Update(double dt)
{
	target = CPlayerInfo::GetInstance()->GetPos();
	Vector3 viewVector = (target - position).Normalized();
	position += viewVector * (float)m_dSpeed * (float)dt;

	// constrain
	Constrain();

	//update the target
	/*if (position.z > 400.0f)
		target.z = position.z * -1;

	else if (position.z < -400.0f)
		target.z = position.z * -1;*/
}

void CEnemy::Constrain(void)
{
	// constrain player within the boundary
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
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		m_pTerrain->GetTerrainHeight(position);
}

void CEnemy::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	modelStack.Rotate(rotate, 0, 1, 0);
	// LOD of enemy
	if (GetLODStatus())
	{
		if (theDetailLevel != NO_DETAILS)
			RenderHelper::RenderMesh(GetLODMesh());
	}
	modelStack.PopMatrix();

}

// Creation of Entities
CEnemy * Create::Enemy(const std::string & _meshName, const Vector3 & _position, 
						const Vector3 & _scale, int _health)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CEnemy* result = new CEnemy(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetHealth(_health);
	result->SetCollider(false);
	EntityManager::GetInstance()->AddEnemy(result, true);
	return result;
}