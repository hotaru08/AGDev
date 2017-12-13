#include "CSceneNode.h"
#include "GraphicsManager.h"
#include "CSceneGraph.h"

CSceneNode::CSceneNode(void)
	: ID(-1)
	, theEntity(NULL)
	, theParent(NULL)
{
}

CSceneNode::~CSceneNode()
{
}

//release all memory for this node and its children
void CSceneNode::Destroy(void)
{
	// destroy the children first
	vector<CSceneNode*>::iterator it;
	for (it = theChildren.begin(); it != theChildren.end(); ++it)
	{
		CSceneNode *temp = *it;
		temp = NULL;
	}
	// reset the pointes for this node
	theEntity = NULL;
	theParent = NULL;
}

// set the id for this node
void CSceneNode::SetID(const int ID)
{
	this->ID = ID;
}

// get the id for this node
int CSceneNode::GetID(void) const
{
	return ID;
}

// set the entity for this node
bool CSceneNode::SetEntity(EntityBase * theEntity)
{
	if (theEntity)
	{
		this->theEntity = theEntity;
		return true;
	}
	return false;
}

// get the id for this node;
EntityBase * CSceneNode::GetEntity(void) const
{
	return theEntity;
}

// set a parent to this node
void CSceneNode::SetParent(CSceneNode * theParent)
{
	this->theParent = theParent;
}

// get parent of this node
CSceneNode * CSceneNode::GetParent(void) const
{
	return theParent;
}

// Get the entity inside this Scene Graph
CSceneNode* CSceneNode::GetEntity(EntityBase* theEntity)
{
	// This version of checking does not check for children
	/*if (this->theEntity == theEntity)
		return this;*/

	// if it is inside this node, then return this node
	if (this->theEntity == theEntity)
		return this;

	// There are children, searching for correct EntityBase
	if (theChildren.size() != 0)
	{
		std::vector<CSceneNode*>::iterator it;
		for (it = theChildren.begin(); it != theChildren.end(); ++it)
		{
			CSceneNode* theNode = (*it)->GetEntity(theEntity);
			if (theNode)
			{
				return theNode;
			}
		}
	}
	return NULL;
}

// get a child from this node using its ID
CSceneNode * CSceneNode::GetEntity(const int ID)
{
	// This does not check for children
	/*if (this->ID == ID)
		return this;*/

	// if it is inside this node, then return this node
	if (this->ID == ID)
		return this;

	// Check the children
	if (theChildren.size() != 0)
	{
		std::vector<CSceneNode*>::iterator it;
		for (it = theChildren.begin(); it != theChildren.end(); ++it)
		{
			// Get the entity of the node
			CSceneNode* theNode = (*it)->GetEntity(ID);
			if (theNode)
			{
				return theNode;
			}
		}
	}
	return NULL;
}

// Return the number of children in this group
int CSceneNode::GetNumOfChild(void)
{
	// Start with this node's children
	int NumOfChild = theChildren.size();
	// Ask the children to feedback how many children they have
	std::vector<CSceneNode*>::iterator it;
	for (it = theChildren.begin(); it != theChildren.end(); ++it)
	{
		NumOfChild += (*it)->GetNumOfChild();
	}
	return NumOfChild;
}

CSceneNode * CSceneNode::AddChild(EntityBase * theEntity)
{
	if (theEntity)
	{
		// Create a new scene node
		CSceneNode* aNewNode = new CSceneNode();
		// setting the entity
		aNewNode->SetEntity(theEntity);
		// setting this as parent
		aNewNode->SetParent(this);
		//generate ID
		aNewNode->SetID(CSceneGraph::GetInstance()->GenerateID());
		// add to vector
		theChildren.push_back(aNewNode);
		// return this new scene node
		return aNewNode;
	}
	return NULL;
}

bool CSceneNode::DeleteChild(EntityBase * theEntity)
{
	if (this->theEntity == theEntity)
	{
		// if this node has children, then we proceed to delete all its children
		if (theChildren.size() != 0)
		{
			// iterate through all the children and delete all of their children and grandchildren etc
			vector<CSceneNode*>::iterator it = theChildren.begin();
			while (it != theChildren.end())
			{
				if ((*it)->DeleteAllChildren())
				{
					cout << "CSceneNode::DeleteChild: Deleted child nodes for the entity" << endl;
				}

				(*it)->GetEntity()->SetIsDone(true);
				delete *it;
				it = theChildren.erase(it);
			}
		}
		return true;
	 }

	else
	{
		// search the children for this particular theEntity
		if (theChildren.size() != 0)
		{
			vector<CSceneNode*>::iterator it = theChildren.begin();
			for(; it != theChildren.end(); ++it)
			{
				//if delete method call above did remove entity
				// then we should proceed to remove this child from our vector of children
				if ((*it)->DeleteChild(theEntity))
				{
					(*it)->GetEntity()->SetIsDone(true);
					delete *it;
					it = theChildren.erase(it);
					break;
				}
			}
		}
	}
	return false;
}

//Delete a child from this node using ID
bool CSceneNode::DeleteChild(const int ID)
{
	// if this node contains theEntity, then we proceed to delete all its children
	if (this->ID == ID)
	{
		// If this node has children, then we proceed to delete them.
		if (theChildren.size() != 0)
		{
			// Iterate through all the children and delete all of their children and grand children etc
			vector <CSceneNode*>::iterator it = theChildren.begin();
			while (it != theChildren.end())
			{
				if ((*it)->DeleteAllChildren())
				{
					cout << "CSceneNode::DeleteChild: Deleted child nodes for ID =" << ID << endl;
				}
				(*it)->GetEntity()->SetIsDone(true);
				delete *it;
				it = theChildren.erase(it);
			}
		}
		return true; // return true to say that this Node contains theEntity
	}
	else
	{
		// Search the children for this particular theEntity
		if (theChildren.size() != 0)
		{
			std::vector<CSceneNode*>::iterator it;
			for (it = theChildren.begin(); it != theChildren.end(); ++it)
			{
				// Check if this child is the one containing theEntity
				if ((*it)->DeleteChild(theEntity))
				{
					// If DeleteChild method call above DID remove theEntity
					// Then we should proceed to removed this child from our vector of children
					(*it)->GetEntity()->SetIsDone(true);
					delete *it;
					theChildren.erase(it);
					break; // Stop deleting since we have already found and deleted theEntity
				}
			}
		}
	}
	return false;
}

// Delete all children from this node without further checking
bool CSceneNode::DeleteAllChildren(void)
{
	bool bResult = false;

	// scan through vector of children
	vector<CSceneNode*>::iterator it = theChildren.begin();
	while (it != theChildren.end())
	{
		// When all the children are deleted for this node
		if ((*it)->DeleteAllChildren())
		{
			cout << "CSceneNode::DeleteChild: Delete Child Nodes." << endl;
		}

		// Set the related entity for deletion
		(*it)->GetEntity()->SetIsDone(true);
		delete *it;
		it = theChildren.erase(it);
		bResult = true;
	}

	return bResult;
}

// Detach a child from this node using the pointer to the node
CSceneNode* CSceneNode::DetachChild(EntityBase* theEntity)
{
	// if it is inside this node, then return this node
	if (this->ID == ID)
		return this;

	// if there is children
	if (theChildren.size() != 0)
	{
		vector <CSceneNode*>::iterator it = theChildren.begin();
		while (it != theChildren.end())
		{
			// detach the current node, with its entity, from its parent 
			CSceneNode* theNode = (*it)->DetachChild(theEntity);
			if (theNode)
			{
				// Remove this node from the children
				theChildren.erase(it);
				return theNode;
			}
			it++;
		}
	}
	return NULL;
}

// Detach a child from this node using its ID
CSceneNode* CSceneNode::DetachChild(const int ID)
{
	// if it is inside this node, then return this node
	if (this->ID == ID)
		return this;

	if (theChildren.size() != 0)
	{
		vector <CSceneNode*>::iterator it = theChildren.begin();
		while (it != theChildren.end())
		{
			// detach the current node, with its entity, from its parent 
			CSceneNode* theNode = (*it)->DetachChild(ID);
			if (theNode)
			{
				// Remove this node from the children
				theChildren.erase(it);
				return theNode;
			}
			it++;
		}
	}
	return NULL;
}

// Update the Scene Graph
void CSceneNode::Update(void)
{
	// Update the Transformation between this node and its children
	if (theUpdateTransformation)
	{
		ApplyTransform(GetUpdateTransformation());
	}

	/*
	Mtx44 orig = GetTransform();
	Mtx44 update = GetUpdateTransform();
	cout << "======================================================================" << endl;
	cout << "Orig" << endl;
	cout << "----------------------------------------------------------------------" << endl;
	cout << "[\t" << orig.a[0] << "\t" << orig.a[4] << "\t" << orig.a[8] << "\t" << orig.a[12] << "\t]" << endl;
	cout << "[\t" << orig.a[1] << "\t" << orig.a[5] << "\t" << orig.a[9] << "\t" << orig.a[13] << "\t]" << endl;
	cout << "[\t" << orig.a[2] << "\t" << orig.a[6] << "\t" << orig.a[10] << "\t" << orig.a[14] << "\t]" << endl;
	cout << "[\t" << orig.a[3] << "\t" << orig.a[7] << "\t" << orig.a[11] << "\t" << orig.a[15] << "\t]" << endl;
	cout << "======================================================================" << endl;

	cout << "======================================================================" << endl;
	cout << "Update" << endl;
	cout << "----------------------------------------------------------------------" << endl;
	cout << "[\t" << update.a[0] << "\t" << update.a[4] << "\t" << update.a[8] << "\t" << update.a[12] << "\t]" << endl;
	cout << "[\t" << update.a[1] << "\t" << update.a[5] << "\t" << update.a[9] << "\t" << update.a[13] << "\t]" << endl;
	cout << "[\t" << update.a[2] << "\t" << update.a[6] << "\t" << update.a[10] << "\t" << update.a[14] << "\t]" << endl;
	cout << "[\t" << update.a[3] << "\t" << update.a[7] << "\t" << update.a[11] << "\t" << update.a[15] << "\t]" << endl;
	cout << "======================================================================" << endl;
	*/

	// Update the children
	std::vector<CSceneNode*>::iterator it;
	for (it = theChildren.begin(); it != theChildren.end(); ++it)
	{
		(*it)->Update();
	}
}
void CSceneNode::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();

	if (theEntity)
	{
		//modelStack.LoadMatrix(this->GetTransform());
		modelStack.MultMatrix(this->GetTransform());

		/*
		Mtx44 Mtx = modelStack.Top();
		cout << "======================================================================" << endl;
		cout << "CSceneNode::PrintSelf" << endl;
		cout << "----------------------------------------------------------------------" << endl;
		cout << "[\t" << Mtx.a[0] << "\t" << Mtx.a[4] << "\t" << Mtx.a[8] << "\t" << Mtx.a[12] << "\t]" << endl;
		cout << "[\t" << Mtx.a[1] << "\t" << Mtx.a[5] << "\t" << Mtx.a[9] << "\t" << Mtx.a[13] << "\t]" << endl;
		cout << "[\t" << Mtx.a[2] << "\t" << Mtx.a[6] << "\t" << Mtx.a[10] << "\t" << Mtx.a[14] << "\t]" << endl;
		cout << "[\t" << Mtx.a[3] << "\t" << Mtx.a[7] << "\t" << Mtx.a[11] << "\t" << Mtx.a[15] << "\t]" << endl;
		cout << "======================================================================" << endl;
		*/


		// Render the entity
		theEntity->Render();
	}

	// Render the children
	std::vector<CSceneNode*>::iterator it;
	for (it = theChildren.begin(); it != theChildren.end(); ++it)
	{
		(*it)->Render();
	}

	modelStack.PopMatrix();
}
// print self for debug purposes
void CSceneNode::PrintSelf(const int numTabs)
{
	//recursive method to print info about itself n children
	if (numTabs == 0)
	{
		cout << "===================================\nStart of the Root::PrintSelf()" << endl;
		cout << "CSceneNode::PrintSelf: ID =" << ID << "/Children =" << theChildren.size() << endl;
		cout << "Printing out the Children: " << endl;
		vector<CSceneNode*>::iterator it = theChildren.begin();
		while (it != theChildren.end())
		{
			(*it)->PrintSelf(numTabs + 1);
			it++;
		}
		cout << "End of theRoot::PrintSelf()\n===================================" << endl;
	}
	else
	{
		for (int i = 0; i < numTabs; i++)
			cout << "\t"; // tab spacing
		cout << "CSceneNode::PrintSelf: ID =" << ID << "/theEntity =" << theEntity << "/Chidren =" << theChildren.size() << endl;
		for (int i = 0; i < numTabs; i++)
			cout << "\t";
		cout << "Printing out the children: " << endl;
		vector<CSceneNode*>::iterator it = theChildren.begin();
		while (it != theChildren.end())
		{
			(*it)->PrintSelf(numTabs + 1);
			it++;
		}
	}
}
