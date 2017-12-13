#include "CLaserBlaster.h"
#include "Laser.h"

CLaserBlaster::CLaserBlaster()
{
}

CLaserBlaster::~CLaserBlaster()
{
}

void CLaserBlaster::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 10;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 10;
	// The current total number of rounds currently carried by this player
	totalRounds = 10;;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 10;

	// The time between shots in milliseconds
	timeBetweenShots = 0.1667;
	// The elapsed time (between shots) in milliseconds
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

// Discharge this weapon
void CLaserBlaster::Discharge(Vector3 pos, Vector3 target, CPlayerInfo * _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			Vector3 Direction = (target - pos).Normalized();
			// Create a laser with a laser mesh, the length is 10.f and mesh is also
			// set at 10.f
			CLaser* aLaser = Create::Laser("laser_ray", 
											pos, 
											Direction,
											10.f, 
											2.f, 
											100.f, 
											_source);

			aLaser->SetIsLaser(true);
			aLaser->SetCollider(true);
			aLaser->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;
		}
	}
}
