#pragma once
#include "GenericEntity.h"
#include "GroundEntity.h"
#include <vector>
#include "Waypoint\Waypoint.h"

using std::vector;

class Mesh;

class CEnemy : public GenericEntity
{
protected:
	enum State
	{
		STATE_MOVE,
		STATE_CHASE,
		STATE_RUN,
	};
	State currentState;
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	double m_dSpeed;
	double m_dAcceleration;
	int health;
	float angle;

	// Vector containing IDs of Waypoints
	vector<int> listOfWaypoints;
	// Current ID of Waypoint
	int m_iWayPointIndex;

private:
	// Meshes
	Mesh* modelMesh;

public:
	CEnemy(Mesh* modelMesh);
	virtual ~CEnemy();

	void Init(void);
	// reset this player instance to default
	void Reset(void);

	//set position
	void SetPos(const Vector3& pos);
	// set target
	void SetTarget(const Vector3& target);
	// set up
	void SetUp(const Vector3& up);
	// set the boundary for the player info 
	void SetBoundary(Vector3 max, Vector3 min);
	// set the terrain for the enemy
	void SetTerrain(GroundEntity* m_pTerrain);

	// Set health
	void SetHealth(int health);
	// Get health
	int GetHealth();

	// get position
	Vector3 GetPos(void) const;
	// get target
	Vector3 GetTarget(void) const;
	// get up 
	Vector3 GetUp(void) const;
	// get the terrain for the enemy
	GroundEntity* GetTerrain(void);
	// Get next Waypoint for this CEnemy
	CWaypoint* GetNextWaypoint(void);

	// update
	void Update(double dt = 0.0333f);
	// constrain the position within the borders 
	void Constrain(void);
	//render
	void Render(void);
};

// Create Enemy Entities
namespace Create
{
	CEnemy* Enemy(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		int _health = 1);
};
