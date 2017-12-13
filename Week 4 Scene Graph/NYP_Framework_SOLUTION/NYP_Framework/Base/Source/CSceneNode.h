#pragma once

#include "Vector3.h"
#include <vector>
using namespace std;

#include "EntityBase.h"
#include "Transform.h"

class CSceneNode : public CTransform
{
public:
	CSceneNode(void);
	virtual ~CSceneNode();

	//release all memory for this node and its children
	void Destroy(void);

	//set the ID for this node
	void SetID(const int ID);
	//get the ID for this node
	int GetID(void) const;

	// set the entity for this node
	bool SetEntity(EntityBase* theEntity);
	// get the id for this ndoe
	EntityBase* GetEntity(void) const;

	// set a parent to this node
	void SetParent(CSceneNode* theParent);
	// get a parent of this node
	CSceneNode* GetParent(void) const;


	// add a child to this node
	CSceneNode* AddChild(EntityBase* theEntity = NULL);
	//delete a child from this node using the pointer to the entity
	bool DeleteChild(EntityBase* theEntity = NULL);
	// delete a child from this node using its ID
	bool DeleteChild(const int ID);
	// delete a all children from this node using its ID
	bool DeleteAllChildren(void);
	// detach a child from this node  using the pointer to the node
	CSceneNode* DetachChild(EntityBase* theEntity = NULL);
	// detach a child from this node using its ID
	CSceneNode* DetachChild(const int ID);


	// get the entity inside this SceneGraph
	CSceneNode* GetEntity(EntityBase* theEntity);
	// get the entity inside this scene graph using its ID
	CSceneNode* GetEntity(const int ID);
	// return the number of children in this group
	int GetNumOfChild(void);

	// Update the Scene Graph
	void Update(void);
	// Render the Scene Graph
	void Render(void);

	// PrintSelf for debug purposes
	void PrintSelf(const int numTabs = 0);

protected:
	int ID;
	EntityBase* theEntity;
	CSceneNode* theParent;
	vector<CSceneNode*> theChildren;
};