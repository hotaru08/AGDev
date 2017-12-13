#pragma once
#include "WeaponInfo.h"

// This class is for the propertities of the laser blaster

class CPlayerInfo; // forward declaration
class CLaserBlaster : public CWeaponInfo
{
public:
	CLaserBlaster();
	virtual ~CLaserBlaster();

	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon 
	void Discharge(Vector3 pos, Vector3 target, CPlayerInfo* _source = NULL); // _source = player to shoot from
};