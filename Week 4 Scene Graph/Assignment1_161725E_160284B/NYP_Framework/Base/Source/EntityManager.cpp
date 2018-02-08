#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "WeaponInfo\Laser.h"
#include "CSceneGraph.h"

#include <iostream>
using namespace std;

// Constructor
EntityManager::EntityManager()
{
}

// Destructor
EntityManager::~EntityManager()
{
}

// Update all entities
void EntityManager::Update(double _dt)
{
	std::list<EntityBase*>::iterator it, it2, it3, it4, it5;

	// Update all entities
	for (it = entityList.begin(); it != entityList.end(); ++it)
	{
		(*it)->Update(_dt);
	}

	// Update Enemies
	for (it2 = EnemyList.begin(); it2 != EnemyList.end(); ++it2)
	{
		(*it2)->Update(_dt);
	}

	// Update Projectiles
	for (it3 = projectileList.begin(); it3 != projectileList.end(); ++it3)
	{
		(*it3)->Update(_dt);
	}

	// Update created list
	for (it4 = createdList.begin(); it4 != createdList.end(); ++it4)
	{
		(*it4)->Update(_dt);
	}

	// Update turret list
	for (it5 = turretList.begin(); it5 != turretList.end(); ++it5)
	{
		(*it5)->Update(_dt);
	}

	//render the scene graph
	CSceneGraph::GetInstance()->Update();

	// Update the Spatial Partition
	if (theSpatialPartition)
		theSpatialPartition->Update();

	// Check for Collision amongst entities with collider properties
	CheckForCollision();
	// Check for Collision amongst entities with collider properties (Enemy)
	CheckForCollisionEnemy();

	// Clean up entities that are done
	it = entityList.begin();
	while (it != entityList.end())
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}

	// Clean Up Enemies that are done
	it2 = EnemyList.begin();
	while (it2 != EnemyList.end())
	{
		if ((*it2)->IsDone())
		{
			// Delete if done
			delete *it2;
			it2 = EnemyList.erase(it2);
		}
		else
		{
			// Move on otherwise
			++it2;
		}
	}

	// Clean Up Projectile that are done
	it3 = projectileList.begin();
	while (it3 != projectileList.end())
	{
		if ((*it3)->IsDone())
		{
			// Delete if done
			delete *it3;
			it3 = projectileList.erase(it3);
		}
		else
		{
			// Move on otherwise
			++it3;
		}
	}

	// Clean Up Projectile that are done
	it4 = createdList.begin();
	while (it4 != createdList.end())
	{
		if ((*it4)->IsDone())
		{
			// Delete if done
			delete *it4;
			it4 = createdList.erase(it4);
		}
		else
		{
			// Move on otherwise
			++it4;
		}
	}

	// Clean Up turret that are done
	it5 = turretList.begin();
	while (it5 != turretList.end())
	{
		if ((*it5)->IsDone())
		{
			// Delete if done
			delete *it5;
			it5 = turretList.erase(it5);
		}
		else
		{
			// Move on otherwise
			++it5;
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Render();
	}

	// Render Enemies
	std::list<EntityBase*>::iterator it2, end2;
	end2 = EnemyList.end();
	for (it2 = EnemyList.begin(); it2 != end2; ++it2)
	{
		(*it2)->Render();
	}

	// Render Projectile
	std::list<EntityBase*>::iterator it3, end3;
	end3 = projectileList.end();
	for (it3 = projectileList.begin(); it3 != end3; ++it3)
	{
		(*it3)->Render();
	}

	//render entities that are created
	std::list<EntityBase*>::iterator it4, end4;
	end4 = createdList.end();
	for (it4 = createdList.begin(); it4 != end4; ++it4)
	{
		(*it4)->Render();
	}

	//render entities that are created
	std::list<EntityBase*>::iterator it5, end5;
	end5 = turretList.end();
	for (it5 = turretList.begin(); it5 != end5; ++it5)
	{
		(*it5)->Render();
	}

	//render the scene graph
	CSceneGraph::GetInstance()->Render();

	// Render the Spatial Partitions ( white lines )
	if (theSpatialPartition)
		theSpatialPartition->Render();
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	//for (it = entityList.begin(); it != end; ++it)
	//{
	//	(*it)->RenderUI();
	//}

	// Render all entities UI
	std::list<EntityBase*>::iterator it2, end2;
	end2 = createdList.end();
	for (it2 = createdList.begin(); it2 != end2; ++it2)
	{
		(*it2)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity, bool bAddToSpatialPartition)
{
	_newEntity->SetIsDone(false);
	entityList.push_back(_newEntity);

	// Choose to add entity to Spatial Partition or not
	if (theSpatialPartition && bAddToSpatialPartition)
		theSpatialPartition->Add(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);

		// Remove Scene Node too
		if (!CSceneGraph::GetInstance()->DeleteNode(_existingEntity))
		{
			cout << "EntityManager::RemoveEntity: Unable to remove" << endl;
		}
		else
		{
			// Remove from Spatial Partition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEntity);
		}
		return true;	
	}
	// Return false if not found
	return false;
}

// Add Enemy to the EnemyList
void EntityManager::AddEnemy(EntityBase * _newEnemy, bool bAddToSpatialPartition)
{
	_newEnemy->SetIsDone(false);
	EnemyList.push_back(_newEnemy);

	// Choose to add entity to Spatial Partition or not
	if (theSpatialPartition && bAddToSpatialPartition)
		theSpatialPartition->Add(_newEnemy);
}

// Remove Enemy from list
bool EntityManager::RemoveEnemy(EntityBase * _existingEnemy)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(EnemyList.begin(), EnemyList.end(), _existingEnemy);

	// Delete the entity if found
	if (findIter != EnemyList.end())
	{
		delete *findIter;
		findIter = EnemyList.erase(findIter);

		// Remove Scene Node too
		if (!CSceneGraph::GetInstance()->DeleteNode(_existingEnemy))
		{
			cout << "EntityManager::RemoveEnemy: Unable to remove" << endl;
		}
		else
		{
			// Remove from Spatial Partition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEnemy);
		}
		return true;
	}
	// Return false if not found
	return false;
}

// Add to Projectile List
void EntityManager::AddProjectile(EntityBase * _newEnemy, bool bAddToSpatialPartition)
{
	_newEnemy->SetIsDone(false);
	projectileList.push_back(_newEnemy);

	// Choose to add entity to Spatial Partition or not
	if (theSpatialPartition && bAddToSpatialPartition)
		theSpatialPartition->Add(_newEnemy);
}

bool EntityManager::RemoveProjectile(EntityBase * _existingEnemy)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(projectileList.begin(), projectileList.end(), _existingEnemy);

	// Delete the entity if found
	if (findIter != projectileList.end())
	{
		delete *findIter;
		findIter = projectileList.erase(findIter);

		// Remove Scene Node too
		if (!CSceneGraph::GetInstance()->DeleteNode(_existingEnemy))
		{
			cout << "EntityManager::RemoveEnemy: Unable to remove" << endl;
		}
		else
		{
			// Remove from Spatial Partition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEnemy);
		}
		return true;
	}
	// Return false if not found
	return false;
}

// Add turret to the turretList
void EntityManager::AddTurret(EntityBase * _newEnemy, bool bAddToSpatialPartition)
{
	_newEnemy->SetIsDone(false);
	turretList.push_back(_newEnemy);

	// Choose to add entity to Spatial Partition or not
	if (theSpatialPartition && bAddToSpatialPartition)
		theSpatialPartition->Add(_newEnemy);
}

// remove turret from the turretList
bool EntityManager::RemoveTurret(EntityBase * _existingEnemy)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(turretList.begin(), turretList.end(), _existingEnemy);

	// Delete the entity if found
	if (findIter != turretList.end())
	{
		delete *findIter;
		findIter = turretList.erase(findIter);

		// Remove Scene Node too
		if (!CSceneGraph::GetInstance()->DeleteNode(_existingEnemy))
		{
			cout << "EntityManager::RemoveEnemy: Unable to remove" << endl;
		}
		else
		{
			// Remove from Spatial Partition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEnemy);
		}
		return true;
	}
	// Return false if not found
	return false;
}

void EntityManager::AddCreated(EntityBase * _newCreated, bool bAddToSpatialPartition)
{
	_newCreated->SetIsDone(false);
	createdList.push_back(_newCreated);

	// Choose to add entity to Spatial Partition or not
	if (theSpatialPartition && bAddToSpatialPartition)
		theSpatialPartition->Add(_newCreated);
}

bool EntityManager::RemoveCreated(EntityBase * _existingCreated)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(createdList.begin(), createdList.end(), _existingCreated);

	// Delete the entity if found
	if (findIter != createdList.end())
	{
		delete *findIter;
		findIter = createdList.erase(findIter);

		// Remove Scene Node too
		if (!CSceneGraph::GetInstance()->DeleteNode(_existingCreated))
		{
			cout << "EntityManager::RemoveCreated: Unable to remove" << endl;
		}
		else
		{
			// Remove from Spatial Partition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingCreated);
		}
		return true;
	}
	// Return false if not found
	return false;
}

// Mark an Entity for deletion
bool EntityManager::MarkForDeletion(EntityBase * _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator it = std::find(entityList.begin(), entityList.end(), _existingEntity);
	// Find the enemy's iterator
	std::list<EntityBase*>::iterator it2 = std::find(EnemyList.begin(), EnemyList.end(), _existingEntity);
	// Find the projectile's iterator
	std::list<EntityBase*>::iterator it3 = std::find(projectileList.begin(), projectileList.end(), _existingEntity);
	// Find the created's iterator
	std::list<EntityBase*>::iterator it4 = std::find(createdList.begin(), createdList.end(), _existingEntity);
	// Find the turret's iterator
	std::list<EntityBase*>::iterator it5 = std::find(turretList.begin(), turretList.end(), _existingEntity);

	// Delete the entity if found
	if (it != entityList.end())
	{
		(*it)->SetIsDone(true);
		return true;
	}

	// Delete the enemy if found
	if (it2 != EnemyList.end())
	{
		(*it2)->SetIsDone(true);
		return true;
	}

	// Delete the projectile if found
	if (it3 != projectileList.end())
	{
		(*it3)->SetIsDone(true);
		return true;
	}

	// Delete the created if found
	if (it4 != createdList.end())
	{
		(*it4)->SetIsDone(true);
		return true;
	}

	// Delete the turret if found
	if (it5 != turretList.end())
	{
		(*it5)->SetIsDone(true);
		return true;
	}

	// Return false if not found
	return false;
}

void EntityManager::SetSpatialPartition(CSpatialPartition * theSpartialPartition)
{
	this->theSpatialPartition = theSpartialPartition;
}

void EntityManager::cleartheList()
{
	createdList.clear();
}

list<EntityBase*>& EntityManager::returnEnemy(void)
{
	return EnemyList;
}

list<EntityBase*>& EntityManager::returnEntity(void)
{
	return entityList;
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{	
	// Check if this object is overlapping that object
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		||
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	{
		return true;
	}

	// Check if that object is overlapping this object
	if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
		||
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	{
		return true;
	}

	// Check if this object is within that object
	if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
		&&
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
		return true;

	// Check if that object is within this object
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		&&
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
		return true;

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// if Radius of bounding sphere of ThisEntity plus Radius of bounding sphere of ThatEntity is 
	// greater than the distance squared between the 2 reference points of the 2 entities,
	// then it could mean that they are colliding with each other.
	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
		DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()) * 2.0)
	{
		return true;
	}

	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// Check for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	// if AABB collision check fails, then we need to check the other corners of the bounding boxes to 
	// Do more collision checks with other points on each bounding box
	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);
//	Vector3 altThatMinAABB = Vector3(thatMinAABB.x, thatMinAABB.y, thatMaxAABB.z);
//	Vector3 altThatMaxAABB = Vector3(thatMaxAABB.x, thatMaxAABB.y, thatMinAABB.z);

	// Check for overlap
	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(void)
{
	// Check for Collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	std::list<EntityBase*>::iterator colliderThatProj, colliderThatEndProj;

	colliderThisEnd = entityList.end();
	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		//check if this entity is a CLaser type
		if ((*colliderThis)->GetIsLaser())
		{
			//Dynamic cast it to a CLaser class
			CLaser* thisEntity = dynamic_cast<CLaser*>(*colliderThis);

			//check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
				// skip if same 
				if (colliderThat == colliderThis)
					continue;
				
				if ((*colliderThat)->HasCollider())
				{
					Vector3 hitPosition = Vector3(0, 0, 0);

					//Get the minAABB and maxAABB for (*colliderThat)
					CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);
					Vector3 thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
					Vector3 thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();

					if (CheckLineSegmentPlane(thisEntity->GetPosition(), 
						thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(),
						thatMinAABB, thatMaxAABB, hitPosition))
					{
						(*colliderThis)->SetIsDone(true);
						(*colliderThat)->SetIsDone(true);

						// Remove from Scene Graph
						CSceneGraph::GetInstance()->DeleteNode(*colliderThis);
						CSceneGraph::GetInstance()->DeleteNode(*colliderThat);
						
					}
				}
			}
		}

		// For Objects that have Collision
		else if ((*colliderThis)->HasCollider())
		{
			// This object was derived from a CCollider class, then it will have Collision Detection methods
			//CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = colliderThis; colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					// This object was derived from a CCollider class, then it will have Collision Detection methods
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
					if (CheckSphereCollision(thisEntity, thatEntity))
					{
						if (CheckAABBCollision(thisEntity, thatEntity))
						{
							thisEntity->SetIsDone(true);
							thatEntity->SetIsDone(true);

							// Remove from Scene Graph
							CSceneGraph::GetInstance()->DeleteNode(*colliderThis);
							CSceneGraph::GetInstance()->DeleteNode(*colliderThat);
						}
					}
				}
			}

			colliderThatEndProj = projectileList.end();
			for (colliderThatProj = projectileList.begin(); colliderThatProj != colliderThatEndProj; ++colliderThatProj)
			{
				/*if (colliderThat == colliderThis)
				continue;*/

				if ((*colliderThatProj)->HasCollider())
				{
					// This object was derived from a CCollider class, then it will have Collision Detection methods
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThatProj);
					if (CheckSphereCollision(thisEntity, thatEntity))
					{
						if (CheckAABBCollision(thisEntity, thatEntity))
						{
							thisEntity->SetIsDone(true);
							thatEntity->SetIsDone(true);
						}
					}
				}
			}
		}
	}
	return false;
}

// Check for collision for enemies
bool EntityManager::CheckForCollisionEnemy(void)
{
	// Check for Collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	// Projectile - Enemy
	std::list<EntityBase*>::iterator colliderThatProj, colliderThatEndProj;

	// Scan thru Enemy List
	colliderThisEnd = EnemyList.end();
	for (colliderThis = EnemyList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		//check if this entity is a CLaser type
		if ((*colliderThis)->GetIsLaser())
		{
			//Dynamic cast it to a CLaser class
			CLaser* thisEntity = dynamic_cast<CLaser*>(*colliderThis);

			//check for collision with another collider class
			colliderThatEnd = EnemyList.end();
			int counter = 0;
			for (colliderThat = EnemyList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
				// skip if same 
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					Vector3 hitPosition = Vector3(0, 0, 0);

					//Get the minAABB and maxAABB for (*colliderThat)
					CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);
					Vector3 thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
					Vector3 thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();

					if (CheckLineSegmentPlane(thisEntity->GetPosition(),
						thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(),
						thatMinAABB, thatMaxAABB, hitPosition))
					{
						//(*colliderThis)->SetIsDone(true);
						//(*colliderThat)->SetIsDone(true);

						//// Remove from Scene Graph
						//CSceneGraph::GetInstance()->DeleteNode(*colliderThis);
						//CSceneGraph::GetInstance()->DeleteNode(*colliderThat);

					}
				}
			}
		}

		// For Objects that have Collision
		else if ((*colliderThis)->HasCollider())
		{
			// This object was derived from a CCollider class, then it will have Collision Detection methods
			//CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

			// Check for collision with another collider class (between Enemy)
			colliderThatEnd = EnemyList.end();
			for (colliderThat = colliderThis; colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					// This object was derived from a CCollider class, then it will have Collision Detection methods
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
					if (CheckSphereCollision(thisEntity, thatEntity))
					{
						if (CheckAABBCollision(thisEntity, thatEntity))
						{
							//thisEntity->SetIsDone(true);
							//thatEntity->SetIsDone(true);

							//// Remove from Scene Graph
							//CSceneGraph::GetInstance()->DeleteNode(*colliderThis);
							//CSceneGraph::GetInstance()->DeleteNode(*colliderThat);

							//cout << "Collided" << endl;
						}
					}
				}
			}

			// Check for collision with another collider class (between Projectile and Enemy)
			colliderThatEndProj = projectileList.end();
			for (colliderThatProj = projectileList.begin(); colliderThatProj != colliderThatEndProj; ++colliderThatProj)
			{
				/*if (colliderThat == colliderThis)
					continue;*/

				if ((*colliderThatProj)->HasCollider())
				{
					// This object was derived from a CCollider class, then it will have Collision Detection methods
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThatProj);
					if (CheckSphereCollision(thisEntity, thatEntity))
					{
						if (CheckAABBCollision(thisEntity, thatEntity))
						{
							thisEntity->SetIsDone(true);
							thatEntity->SetIsDone(true);

							//// Remove from Scene Graph
							CSceneGraph::GetInstance()->DeleteNode(*colliderThis);//main body of enemy
							CSceneGraph::GetInstance()->DeleteNode(*colliderThatProj);//proj
						}
					}
				}
			}
		}
	}
	return false;
}

bool EntityManager::CheckForCollisionProjectile(void)
{
	return false;
}

// check where a line segment between two positions intersects a plane
bool EntityManager::GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 & Hit)
{
	if ((fDst1 * fDst2) >= 0.0f)
		return false;

	if (fDst1 == fDst2)
		return false;

	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return true;
}

//check for intersection between a line and plane
bool EntityManager::CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end, Vector3 minAABB, Vector3 maxAABB, Vector3 & Hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x,
		line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y -
			minAABB.y, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - minAABB.z, line_end.z -
			minAABB.z, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 3))
		|| (GetIntersection(line_start.x - maxAABB.x, line_end.x -
			maxAABB.x, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - maxAABB.y, line_end.y -
			maxAABB.y, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - maxAABB.z, line_end.z -
			maxAABB.z, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 3)))
		return true;
	return false;
}

bool EntityManager::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
	return false;
}
