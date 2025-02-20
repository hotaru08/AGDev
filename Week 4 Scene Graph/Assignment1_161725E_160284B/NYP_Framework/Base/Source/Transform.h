#pragma once
#include "Mtx44.h"
#include "CUpdateTransformation.h"

class CTransform
{
protected:
	Mtx44 Mtx, DefaultMtx;
	CUpdateTransformation* theUpdateTransformation;

public:
	enum AXIS
	{
		X_AXIS = 0,
		Y_AXIS,
		Z_AXIS,
		NUM_AXIS
	};

	// default constructor
	CTransform(void);
	//Overloaded constructor
	CTransform(const float dx, const float dy, const float dz);
	~CTransform(void);
	
	// Apply a translation to the Transformation Matrix
	void ApplyTranslate(const float dx, const float dy, const float dz);
	// Get the translation from the Transformation Matrix
	void GetTranslate(float& x, float& y, float& z);
	// Apply a rotation to the Transformation Matrix
	void ApplyRotate(const float angle, const float rx, const float ry, const float rz);
	// Get the rotation from the Transformation Matrix
	float GetRotate(const AXIS theAxis);
	// Set the scale of the Transformation Matrix
	void SetScale(const float sx, const float sy, const float sz);
	// Get the scale from the Transformation Matrix
	void GetScale(float& x, float& y, float& z);

	// Apply a Transformation Matrix to the Transformation Matrix here
	void ApplyTransform(Mtx44 newMTX);
	// Reset the transformation matrix to identity matrix
	void Reset(void); //reset to identity
	// Get the transformation matrix
	Mtx44 GetTransform(void);

	// Set the Update Transformation
	void SetUpdateTransformation(CUpdateTransformation* theUpdateTransforamtion = NULL);
	// Get the Update Transformation Matrix
	Mtx44 GetUpdateTransformation(void);

	// Print Self
	void PrintSelf(void) const;
};