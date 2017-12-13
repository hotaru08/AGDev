#include "Transform.h"
#include <iostream>

using namespace std;

CTransform::CTransform(void)
	: theUpdateTransformation(NULL)
{
	Mtx.SetToIdentity();
	DefaultMtx.SetToIdentity();
}

CTransform::CTransform(const float dx, const float dy, const float dz)
{
	Mtx.SetToTranslation(dx, dy, dz);
}

CTransform::~CTransform(void)
{
	// Clear instance
	if (theUpdateTransformation)
	{
		delete theUpdateTransformation;
		theUpdateTransformation = NULL;
	}
}

// ============================= Translation Section
void CTransform::ApplyTranslate(const float dx, const float dy, const float dz)
{
	Mtx44 TempMtx; // temp matrix used to apply to object
	TempMtx.SetToTranslation(dx, dy, dz);
	Mtx = Mtx * TempMtx; // apply tempmtx to mtx using now
}

void CTransform::GetTranslate(float & x, float & y, float & z)
{
	x = Mtx.a[12]; // last row x
	y = Mtx.a[13]; // last row y
	z = Mtx.a[14]; // last row z
}

// ============================= Rotation Section
void CTransform::ApplyRotate(const float angle, const float rx, const float ry, const float rz)
{
	Mtx44 tempMtx;
	tempMtx.SetToRotation(angle, rx, ry, rz);
	Mtx = Mtx * tempMtx;
}

float CTransform::GetRotate(const AXIS theAxis)
{
	if (theAxis == X_AXIS)
	{
		float y = acos(Mtx.a[5]);// equal to cos a
		float z = asin(-Mtx.a[9]); // equals to sin a

		if (abs(y - z) < Math::EPSILON)
		{
			return y;
		}
	}

	else if (theAxis == Y_AXIS)
	{
		float x = acos(Mtx.a[0]); // equals to cos a
		float z = asin(Mtx.a[8]); // equals to -sin a

		if (abs(x - z) < Math::EPSILON)
		{
			return x;
		}
	}

	else
	{
		float x = acos(Mtx.a[0]);
		float y = acos(-Mtx.a[4]);

		if (abs(x - y) < Math::EPSILON)
			return x;
	}

	return 0.0f;
}

// ============================= Scaling Section
void CTransform::SetScale(const float sx, const float sy, const float sz)
{
	float scaleX = sx, scaleY = sy, scaleZ = sz;
	// setting to be 1.0f so it can render
	if (scaleX == 0.0f)
		scaleX = 1.0f;
	if (scaleY == 0.0f)
		scaleY = 1.0f;
	if (scaleZ == 0.0f)
		scaleZ = 1.0f;

	Mtx.SetToScale(scaleX, scaleY, scaleZ);
}

void CTransform::GetScale(float & x, float & y, float & z)
{
	x = Mtx.a[0];
	y = Mtx.a[5];
	z = Mtx.a[10];
}

// apply a transformation matrix to the transformation matrix here
void CTransform::ApplyTransform(Mtx44 newMTX)
{
	Mtx = Mtx * newMTX;
}

// reset the transformation matrix to identity matrix
void CTransform::Reset(void)
{
	Mtx.SetToIdentity();
}

// get the transformation matrix
Mtx44 CTransform::GetTransform(void)
{
	return Mtx;
}

// Set the Update Transformation
void CTransform::SetUpdateTransformation(CUpdateTransformation * theUpdateTransforamtion)
{
	this->theUpdateTransformation = theUpdateTransforamtion;
}

// Get the Update Transformation
Mtx44 CTransform::GetUpdateTransformation(void)
{
	if (theUpdateTransformation == NULL)
		return DefaultMtx;

	// Update theUpdateTransformation
	theUpdateTransformation->Update();

	// recursive
	return theUpdateTransformation->GetUpdateTransformation();
}

void CTransform::PrintSelf(void) const
{
	cout << "======================================================================"
		<< endl;
	cout << "CTransform::PrintSelf" << endl;
	cout << "----------------------------------------------------------------------"
		<< endl;
	cout << "[\t" << Mtx.a[0] << "\t" << Mtx.a[4] << "\t" << Mtx.a[8] << "\t" <<
		Mtx.a[12] << "\t]" << endl;
	cout << "[\t" << Mtx.a[1] << "\t" << Mtx.a[5] << "\t" << Mtx.a[9] << "\t" <<
		Mtx.a[13] << "\t]" << endl;
	cout << "[\t" << Mtx.a[2] << "\t" << Mtx.a[6] << "\t" << Mtx.a[10] << "\t" <<
		Mtx.a[14] << "\t]" << endl;
	cout << "[\t" << Mtx.a[3] << "\t" << Mtx.a[7] << "\t" << Mtx.a[11] << "\t" <<
		Mtx.a[15] << "\t]" << endl;
	cout << "======================================================================"
		<< endl;
}