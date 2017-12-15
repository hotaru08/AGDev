#ifndef TURRET_H
#define TURRET_H

#include "../../GenericEntity.h"
#include "../../GroundEntity.h"
#include "../../Projectile/Projectile.h"

class Turret : public GenericEntity
{
protected:
	Mesh* modelMesh;

	enum TURRET_TYPE
	{
		NONE = 0,
		TURRET,
		RAPIDFIRE_TURRET,
		WIDERANGE_TURRET,

		NUM_TOTAL
	};
	TURRET_TYPE type;

	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up, rotate;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;
	
	// Bool indicating if able to fire
	bool m_bFire;


public:
	Turret(Mesh* _modelMesh);
	virtual ~Turret();

	// Set Mesh of object
	void SetMesh(Mesh* mesh);

	void Init(void);
	void Update(double dt);
	void Render(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Get position
	Vector3 GetPos(void) const;

	// Set target
	void SetTarget(const Vector3& target);
	// Get target
	Vector3 GetTarget(void) const;

	// Set Up
	void SetUp(const Vector3& up);
	// Get Up
	Vector3 GetUp(void) const;

	// Set the boundary 
	void SetBoundary(Vector3 max, Vector3 min);

	// Set the terrain 
	void SetTerrain(GroundEntity* m_pTerrain);
	// Get the terrain 
	GroundEntity* GetTerrain(void);

	// Set type
	void SetType(int type);
	// Get type of enemy
	int GetType();

	// Set Able to fire
	void SetFire(bool fire);
	// Get Able to Fire
	bool GetFire();

	// Constrain the position within the borders
	void Constrain(void);

	// CoolDown between shots
	double m_dCoolDown;
};

namespace Create
{
	Turret* Turret3D(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		const int type = 1);
}

#endif