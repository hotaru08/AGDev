#include "LevelOfDetail.h"
#include "MeshBuilder.h"

/********************************************************************************
Constructor
********************************************************************************/
CLevelOfDetails::CLevelOfDetails()
	: modelMesh_HighDetails(NULL)
	, modelMesh_MidDetails(NULL)
	, modelMesh_LowDetails(NULL)
	, m_bActive(false)
	, theDetailLevel(HIGH_DETAILS)
{
}
/********************************************************************************
Destructor
********************************************************************************/
CLevelOfDetails::~CLevelOfDetails()
{
	modelMesh_HighDetails = NULL;
	modelMesh_MidDetails = NULL;
	modelMesh_LowDetails = NULL;
}

/********************************************************************************
Initialise the LOD system
********************************************************************************/
bool CLevelOfDetails::InitLOD(const std::string & _meshName_High, const std::string & _meshName_Mid, const std::string & _meshName_Low)
{
	// Retrieve the Meshes from MeshBuilder's library and assign to the 3 Mesh pointers
	modelMesh_HighDetails = MeshBuilder::GetInstance()->GetMesh(_meshName_High); // high detail mesh
	modelMesh_MidDetails = MeshBuilder::GetInstance()->GetMesh(_meshName_Mid); // mid detail mesh
	modelMesh_LowDetails = MeshBuilder::GetInstance()->GetMesh(_meshName_Low); // low detail mesh

	// if any of the mesh is not loaded, then return false and avoid setting the LOD to active
	if ((modelMesh_HighDetails == nullptr) ||
		modelMesh_MidDetails == nullptr ||
		modelMesh_LowDetails == nullptr)
	{
		return false;
	}

	// set the LOD to active
	SetLODStatus(true);
	return true;
}


/********************************************************************************
Destroy the LOD system
********************************************************************************/
bool CLevelOfDetails::DestroyLOD(void)
{
	if (modelMesh_HighDetails)
	{
		delete modelMesh_HighDetails;
		modelMesh_HighDetails = NULL;
	}

	if (modelMesh_MidDetails)
	{
		delete modelMesh_MidDetails;
		modelMesh_MidDetails = NULL;
	}

	if (modelMesh_LowDetails)
	{
		delete modelMesh_LowDetails;
		modelMesh_LowDetails = NULL;
	}

	return false;
}

/********************************************************************************
Set the status of the LOD
********************************************************************************/
void CLevelOfDetails::SetLODStatus(const bool bActive)
{
	// If the status is not actve, render original Mesh in EntityBase
	m_bActive = bActive;
}

/********************************************************************************
Get the LOD Status
********************************************************************************/
bool CLevelOfDetails::GetLODStatus(void) const
{
	return m_bActive;
}

/********************************************************************************
Set the Mesh for a specified detail level
********************************************************************************/
bool CLevelOfDetails::SetLODMesh(Mesh * theMesh, const DETAIL_LEVEL theDetailLevel)
{
	// Detail level is set high
	if (theDetailLevel == HIGH_DETAILS)
		modelMesh_HighDetails = theMesh;

	// Detail level is set mid
	else if (theDetailLevel == MID_DETAILS)
		modelMesh_MidDetails = theMesh;

	// Detail level is set low
	else if (theDetailLevel == LOW_DETAILS)
		modelMesh_LowDetails = theMesh;
	
	// Detail is none
	else
		return false;

	return true;
}

/********************************************************************************
Get the Mesh based on the current detail level (Render objects in EntityManager)
********************************************************************************/
Mesh * CLevelOfDetails::GetLODMesh(void) const
{
	if (theDetailLevel == HIGH_DETAILS)
		return modelMesh_HighDetails;
	else if (theDetailLevel == MID_DETAILS)
		return modelMesh_MidDetails;
	else if (theDetailLevel == LOW_DETAILS)
		return modelMesh_LowDetails;

	return NULL;
}

/********************************************************************************
Get the Mesh based on the specified detail level
********************************************************************************/
Mesh * CLevelOfDetails::GetLODMesh(const DETAIL_LEVEL theDetailLevel) const
{
	if (theDetailLevel == HIGH_DETAILS)
		return modelMesh_HighDetails;
	else if (theDetailLevel == MID_DETAILS)
		return modelMesh_MidDetails;
	else if (theDetailLevel == LOW_DETAILS)
		return modelMesh_LowDetails;

	return NULL;
}

/********************************************************************************
Set the current detail level
********************************************************************************/
bool CLevelOfDetails::SetDetailLevel(const DETAIL_LEVEL theDetailLevel)
{
	if ((theDetailLevel >= NO_DETAILS) && (theDetailLevel < NUM_DETAIL_LEVEL))
	{
		this->theDetailLevel = theDetailLevel;
		return true;
	}
	return false;
}

/********************************************************************************
Get the current detail level
********************************************************************************/
int CLevelOfDetails::GetDetailLevel(void) const
{
	return theDetailLevel;
}











