#include "CGrenade.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GL\glew.h"
#include "../PlayerInfo/PlayerInfo.h"
#include <iostream>
#include "../CSceneGraph.h"

using namespace std;

CGrenade::CGrenade(void)
	: CProjectile(NULL)
	, m_fGravity(-10.0f)
	, m_fElapsedTime(0.0f)
	, m_pTerrain(NULL)
{
}

CGrenade::CGrenade(Mesh* _modelMesh)
	: CProjectile(_modelMesh)
	, m_fGravity(-10.0f)
	, m_fElapsedTime(0.0f)
	, m_pTerrain(NULL)
{
}

CGrenade::~CGrenade(void)
{
	m_pTerrain = NULL; // Don't delete this as the terrain is deleted in CPlayerInfo
	modelMesh = NULL;
	theSource = NULL;
}

void CGrenade::Update(double dt)
{
	if (!m_bStatus)
		return;

	// Update TimeLife of projectile. Set to inactive if too long
	m_fLifetime -= static_cast<float>(dt);
	if (m_fLifetime < 0.0f)
	{
		SetStatus(false);
		SetIsDone(true);	// This method informs EntityManager to remove this instance

							// Check the SpatialPartition to destroy nearby objects
		vector<EntityBase*> ExportList = CSpatialPartition::GetInstance()->GetObjects(position, 1000.0f);
		for (int i = 0; i < ExportList.size(); ++i)
		{
			// Remove from Scene Graph
			if (CSceneGraph::GetInstance()->DeleteNode(ExportList[i]) == true)
			{
				cout << "*** This Entity removed ***" << endl;
			}


		}
		return;
	}

	// Check if the Grenade is already on the ground
	if (position.y >= m_pTerrain->GetTerrainHeight(position) - 10.0f + Math::EPSILON)
	{
		// Update Position
		// Elapsed time is the time since the release of the gernade
		m_fElapsedTime += static_cast<float>(dt);

		// Update Position
		position.Set(position.x + (float)(theDirection.x * m_fElapsedTime * m_fSpeed),
			position.y + (float)(theDirection.y * m_fElapsedTime * m_fSpeed) + (0.5 * m_fGravity * m_fElapsedTime * m_fElapsedTime),
			position.z + (float)(theDirection.z * m_fElapsedTime * m_fSpeed));

		if (position.y < m_pTerrain->GetTerrainHeight(position) - 10.0f)
		{
			position.y = m_pTerrain->GetTerrainHeight(position) - 10.0f;
			m_fSpeed = 0.0f;
			return;
		}
	}
}

void CGrenade::SetTerrain(GroundEntity * m_pTerrain)
{
	this->m_pTerrain = m_pTerrain;
}

CGrenade * Create::Grenade(const std::string & _meshName, const Vector3 & _position, const Vector3 & _direction,
	const float m_fLifetime, const float m_fSpeed, CPlayerInfo * _source)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CGrenade* result = new CGrenade(modelMesh);
	result->Set(_position, _direction, m_fLifetime, m_fSpeed);
	result->SetStatus(true);
	result->SetCollider(true);
	result->SetSource(_source);
	result->SetTerrain(_source->GetTerrain());
	EntityManager::GetInstance()->AddEntity(result, true);
	return result;

}
