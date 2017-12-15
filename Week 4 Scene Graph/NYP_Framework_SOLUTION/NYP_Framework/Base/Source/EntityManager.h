#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "Vector3.h"
#include "SpatialPartition.h"

class EntityBase;

class EntityManager : public Singleton<EntityManager>
{
	friend Singleton<EntityManager>;
public:
	void Update(double _dt);
	void Render();
	void RenderUI();

	// Add and Remove from entityList
	void AddEntity(EntityBase* _newEntity, bool bAddToSpatialPartition = false);
	bool RemoveEntity(EntityBase* _existingEntity);

	// Add and Remove from EnemyList
	void AddEnemy(EntityBase* _newEnemy, bool bAddToSpatialPartition = false);
	bool RemoveEnemy(EntityBase* _existingEnemy);

	// Add and Remove from projectileList
	void AddProjectile(EntityBase* _newEnemy, bool bAddToSpatialPartition = false);
	bool RemoveProjectile(EntityBase* _existingEnemy);

	// Add and Remove from createdList
	void AddCreated(EntityBase* _newCreated, bool bAddToSpatialPartition = false);
	bool RemoveCreated(EntityBase* _existingCreated);

	// Spartial Partitioning
	bool MarkForDeletion(EntityBase* _existingEntity);
	void SetSpatialPartition(CSpatialPartition* theSpartialPartition);

	list<EntityBase*>&returnEnemy(void);
	list<EntityBase*>&returnEntity(void);

private:
	EntityManager();
	virtual ~EntityManager();

	// Check for overlap
	bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	bool CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check if this entity collided with another entity, but both must have collider
	bool CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);

	// Check if any Collider is colliding with another Collider
	bool CheckForCollision(void);
	// Check if any Collider is colliding with another Collider (Enemy)
	bool CheckForCollisionEnemy(void);
	// Check if any Collider is colliding with another Collider (Projectile)
	bool CheckForCollisionProjectile(void);

	// check for intersection between a line segment and a plane
	bool GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit);

	//check fo intersection between a line segment and a plane
	bool CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end, Vector3 minAABB, Vector3 maxAABB, Vector3 &Hit);

	//check if any collider is colliding with another collider
	bool InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);

	std::list<EntityBase*> entityList;
	std::list<EntityBase*> createdList;
	std::list<EntityBase*> EnemyList;
	std::list<EntityBase*> projectileList;

	// Handler to Spatial Partition class
	CSpatialPartition* theSpatialPartition;
};

#endif // ENTITY_MANAGER_H