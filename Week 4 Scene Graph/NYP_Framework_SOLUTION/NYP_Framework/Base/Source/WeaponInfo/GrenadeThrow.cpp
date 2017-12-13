#include "GrenadeThrow.h"
#include "../Projectile/CGrenade.h"
CGrenadeThrow::CGrenadeThrow()
{
}
CGrenadeThrow::~CGrenadeThrow()
{
}
// Initialise this instance to default values
void CGrenadeThrow::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();
	// The number of ammunition in a magazine for this weapon
	magRounds = 1;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 1;
	// The current total number of rounds currently carried by this player
	totalRounds = 2;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 2;
	// The time between shots
	timeBetweenShots = 1.0;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

void CGrenadeThrow::Discharge(Vector3 position, Vector3 target, CPlayerInfo * _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			Vector3 Direction = (target - position).Normalized();
			CGrenade* aGrenade = Create::Grenade("sphere",
				position,
				Direction,
				7.f,
				10.f,
				_source);

			//aGrenade->Set(true);
			aGrenade->SetCollider(true);
			aGrenade->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;
		}
	}
}
