#include "SpatialPartition.h"
#include "stdio.h"
#include "Collider\Collider.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "LevelOfDetail\LevelOfDetail.h"
#include "PlayerInfo\PlayerInfo.h"
#include "MeshBuilder.h"

template <typename T> vector<T> concat(vector<T> &a, vector<T> &b) {
	vector<T> ret = vector<T>();
	copy(a.begin(), a.end(), back_inserter(ret));
	copy(b.begin(), b.end(), back_inserter(ret));
	return ret;
}

// Allocating and initializing CPlayerInfo's static data member.  
// The pointer is allocated but not the object's constructor.
CSpatialPartition *CSpatialPartition::sp_instance = 0;

/********************************************************************************
Constructor
********************************************************************************/
CSpatialPartition::CSpatialPartition(void)
	: theGrid(NULL)
	, xSize(0)
	, zSize(0)
	, xGridSize(0)
	, zGridSize(0)
	, xNumOfGrid(0)
	, zNumOfGrid(0)
	, yOffset(0.0f)
	, _meshName("")
	, theCamera(NULL)
	, renderPos(false)
{
}

/********************************************************************************
Destructor
********************************************************************************/
CSpatialPartition::~CSpatialPartition(void)
{
	theCamera = NULL;
	delete[] theGrid;
}

/********************************************************************************
Initialise the spatial partition
********************************************************************************/
bool CSpatialPartition::Init(const int xGridSize, const int zGridSize,
	const int xNumOfGrid, const int zNumOfGrid,
	const float yOffset)
{
	if ((xGridSize>0) && (zGridSize>0)
		&& (xNumOfGrid>0) && (zNumOfGrid>0))
	{
		this->xNumOfGrid = xNumOfGrid;
		this->zNumOfGrid = zNumOfGrid;
		this->xGridSize = xGridSize;
		this->zGridSize = zGridSize;
		this->xSize = xGridSize * xNumOfGrid;
		this->zSize = zGridSize * zNumOfGrid;
		this->yOffset = yOffset;

		// Create an array of grids
		theGrid = new CGrid[xNumOfGrid*zNumOfGrid];

		// Initialise the array of grids
		for (int i = 0; i<xNumOfGrid; i++)
		{
			for (int j = 0; j<zNumOfGrid; j++)
			{
				theGrid[i*zNumOfGrid + j].Init(i, j, xGridSize, zGridSize, (xSize >> 1), (zSize >> 1));
			}
		}

		// Assign a Mesh to each Grid if available.
		ApplyMesh();

		// Create a migration list vector
		MigrationList.clear();

		return true;
	}
	return false;
}

/********************************************************************************
Set a particular grid's Mesh
********************************************************************************/
void CSpatialPartition::SetMesh(const std::string& _meshName)
{
	this->_meshName = _meshName;

	// Assign a Mesh to each Grid if available.
	ApplyMesh();
}

/********************************************************************************
ApplyMesh
********************************************************************************/
void CSpatialPartition::ApplyMesh(void)
{
	if (_meshName.size() != 0)
	{
		for (int i = 0; i<xNumOfGrid; i++)
		{
			for (int j = 0; j<zNumOfGrid; j++)
			{
				theGrid[i*zNumOfGrid + j].SetMesh(_meshName);
			}
		}
	}
}

/********************************************************************************
Update the spatial partition
********************************************************************************/
void CSpatialPartition::Update(void)
{
	for (int i = 0; i<xNumOfGrid; i++)
	{
		for (int j = 0; j < zNumOfGrid; j++)
		{
			// Nested for loop, check if objects has left grids
			theGrid[i*zNumOfGrid + j].Update(&MigrationList);


			if (IsVisible(theCamera->GetCameraPos(),
				theCamera->GetCameraTarget() - theCamera->GetCameraPos(),
				i, j))
			{
				// calculate LOD for this CGrid
				float distance = CalculateDistanceSquare(&theCamera->GetCameraPos(), i, j);
				if (distance < LevelOfDetails_Distances[0])
				{
					theGrid[i * zNumOfGrid + j].SetDetailLevel(CLevelOfDetails::HIGH_DETAILS);
				}

				else if (distance < LevelOfDetails_Distances[1])
				{
					theGrid[i * zNumOfGrid + j].SetDetailLevel(CLevelOfDetails::MID_DETAILS);
				}

				else
				{
					theGrid[i * zNumOfGrid + j].SetDetailLevel(CLevelOfDetails::LOW_DETAILS);
				}
			}

			/*else
				theGrid[i * zNumOfGrid + j].SetDetailLevel(CLevelOfDetails::NO_DETAILS);*/
		}
	}

	// If there are objects due for migration, then process them
	if (MigrationList.empty() == false)
	{
		// Check each object to see if they are no longer in this grid
		for (int i = 0; i < MigrationList.size(); ++i)
		{
			Add(MigrationList[i]);
		}

		MigrationList.clear();
	}

	gridX = ((CPlayerInfo::GetInstance()->GetPos().x >= 0) ? (
		int)((CPlayerInfo::GetInstance()->GetPos().x / xGridSize)) : 
		(int)(((CPlayerInfo::GetInstance()->GetPos().x - xGridSize) / xGridSize))) * xGridSize + (xGridSize / 2);

	gridZ = ((CPlayerInfo::GetInstance()->GetPos().z >= 0) ? 
		(int)((CPlayerInfo::GetInstance()->GetPos().z / zGridSize)) : 
		(int)(((CPlayerInfo::GetInstance()->GetPos().z - zGridSize) / zGridSize))) * zGridSize + (zGridSize / 2);
}

/********************************************************************************
Render the spatial partition
********************************************************************************/
void CSpatialPartition::Render(Vector3* theCameraPosition)
{
	// Render the Spatial Partitions
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	if (renderPos)
	{
		modelStack.PushMatrix();
		modelStack.Translate(gridX, yOffset, gridZ);
		modelStack.Scale(xGridSize, 0.1, zGridSize);
		//modelStack.Rotate(90, 1, 0, 0);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("gridpos"));
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(0.0f, yOffset, 0.0f);
	for (int i = 0; i < xNumOfGrid; i++)
	{
		for (int j = 0; j < zNumOfGrid; j++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(xGridSize*i - (xSize >> 1), 0.0f, zGridSize*j - (zSize >> 1));
			modelStack.PushMatrix();
			modelStack.Scale(xGridSize, 1.0f, zGridSize);
			modelStack.Rotate(-90, 1, 0, 0);
			theGrid[i*zNumOfGrid + j].Render();
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
	}

	modelStack.PopMatrix();
}

/********************************************************************************
Get xSize of the entire spatial partition
********************************************************************************/
int CSpatialPartition::GetxSize(void) const
{
	return xSize;
}
/********************************************************************************
Get zSize of the entire spatial partition
********************************************************************************/
int CSpatialPartition::GetzSize(void) const
{
	return zSize;
}
/********************************************************************************
Get xSize
********************************************************************************/
int CSpatialPartition::GetxGridSize(void) const
{
	return xGridSize;
}
/********************************************************************************
Get zNumOfGrid
********************************************************************************/
int CSpatialPartition::GetzGridSize(void) const
{
	return zGridSize;
}
/********************************************************************************
Get xNumOfGrid
********************************************************************************/
int CSpatialPartition::GetxNumOfGrid(void) const
{
	return xNumOfGrid;
}
/********************************************************************************
Get zNumOfGrid
********************************************************************************/
int CSpatialPartition::GetzNumOfGrid(void) const
{
	return zNumOfGrid;
}

/********************************************************************************
Get a particular grid
********************************************************************************/
CGrid CSpatialPartition::GetGrid(const int xIndex, const int yIndex) const
{
	return theGrid[xIndex*zNumOfGrid + yIndex];
}

/********************************************************************************
Get vector of objects from this Spatial Partition
********************************************************************************/
vector<EntityBase*> CSpatialPartition::GetObjects(Vector3 position, const float radius)
{
	// Get the indices of the object's position
	int xIndex = (((int)position.x - (-xSize >> 1)) / (xSize / xNumOfGrid));
	int zIndex = (((int)position.z - (-zSize >> 1)) / (zSize / zNumOfGrid));

	return theGrid[xIndex*zNumOfGrid + zIndex].GetListOfObject();
}

/********************************************************************************
Add a new object model
********************************************************************************/
void CSpatialPartition::Add(EntityBase* theObject)
{
	// Get the indices of the object's position
	int xIndex = (((int)theObject->GetPosition().x - (-xSize >> 1)) / (xSize / xNumOfGrid));
	int zIndex = (((int)theObject->GetPosition().z - (-zSize >> 1)) / (zSize / zNumOfGrid));

	// Add them to each grid
	if (((xIndex >= 0) && (xIndex<xNumOfGrid)) && ((zIndex >= 0) && (zIndex<zNumOfGrid)))
	{
		theGrid[xIndex*zNumOfGrid + zIndex].Add(theObject);
	}
}

// Remove but not delete object from this grid
void CSpatialPartition::Remove(EntityBase* theObject)
{
	/*
	// Get the indices of the object's position
	int xIndex = (((int)theObject->GetPosition().x - (-xSize >> 1)) / (xSize / xNumOfGrid));
	int zIndex = (((int)theObject->GetPosition().z - (-zSize >> 1)) / (zSize / zNumOfGrid));

	// Add them to each grid
	if (((xIndex >= 0) && (xIndex<xNumOfGrid)) && ((zIndex >= 0) && (zIndex<zNumOfGrid)))
	{
	theGrid[xIndex*zNumOfGrid + zIndex].Remove(theObject);
	}
	*/
}

/********************************************************************************
Calculate the squared distance from camera to a grid's centrepoint
********************************************************************************/
float CSpatialPartition::CalculateDistanceSquare(Vector3* theCameraPosition, const int xIndex, const int zIndex)
{
	float xDistance = (xIndex * xGridSize + (xGridSize >> 1) - (xSize >> 1)) - theCameraPosition->x;
	float zDistance = (zIndex * zGridSize + (zGridSize >> 1) - (zSize >> 1)) - theCameraPosition->z;

	return (float)(xDistance*xDistance + zDistance*zDistance);
}


/********************************************************************************
PrintSelf
********************************************************************************/
void CSpatialPartition::PrintSelf() const
{
	cout << "******* Start of CSpatialPartition::PrintSelf() **********************************" << endl;
	cout << "xSize\t:\t" << xSize << "\tzSize\t:\t" << zSize << endl;
	cout << "xNumOfGrid\t:\t" << xNumOfGrid << "\tzNumOfGrid\t:\t" << zNumOfGrid << endl;
	if (theGrid)
	{
		cout << "theGrid : OK" << endl;
		cout << "Printing out theGrid below: " << endl;
		for (int i = 0; i<xNumOfGrid; i++)
		{
			for (int j = 0; j<zNumOfGrid; j++)
			{
				theGrid[i*zNumOfGrid + j].PrintSelf();
			}
		}
	}
	else
		cout << "theGrid : NULL" << endl;
	cout << "******* End of CSpatialPartition::PrintSelf() **********************************" << endl;
}

/********************************************************************************
Set LOD distance to switch resolution
********************************************************************************/
void CSpatialPartition::SetLevelOfDetails(const float distance_High2Mid, const float distance_Mid2Low)
{
	this->LevelOfDetails_Distances[0] = distance_High2Mid; // first index store distance to change from high res to mid
	this->LevelOfDetails_Distances[1] = distance_Mid2Low; // first index store distance to change from mid res to low
}

/********************************************************************************
Check if a CGrid is visible to the Camera using position and direction
********************************************************************************/
bool CSpatialPartition::IsVisible(Vector3 theCameraPosition,
	Vector3 theCameraDirection,
	const int xIndex, const int zIndex)
{
	float xDistance = (xGridSize * xIndex + (xGridSize >> 1) - (xSize >> 1)) - theCameraPosition.x;
	float zDistance = (zGridSize * zIndex + (zGridSize >> 1) - (zSize >> 1)) - theCameraPosition.z;
	Vector3 gridCentre(xDistance, 0, zDistance);
	if (theCameraDirection.Dot(gridCentre) < 0) // check whether the angle is between 0 and 90 degrees
		return false;
	return true;
}

void CSpatialPartition::SetCamera(FPSCamera * _cameraPtr)
{
	theCamera = _cameraPtr;
}

void CSpatialPartition::RemoveCamera(void)
{
	delete theCamera;
	theCamera = NULL;
}