#include "CVector.h"
#include <math.h>

CVector::CVector()
{
	x = .0f;
	y = .0f;
	z = .0f;
}

float CVector::Length()
{
	return sqrt(x*x + y*y + z*z);
}

void CVector::Normalize()
{
	float len = Length();
	if (len > 0)
	{
		x /= len;
		y /= len;
		z /= len;
	}
}