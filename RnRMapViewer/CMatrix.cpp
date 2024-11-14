#include "CMatrix.h"
#include <math.h>

CMatrix::CMatrix()
{
	SetIdentity();
}

CMatrix::CMatrix(const CMatrix &matrix)
{
	memcpy(this, &matrix, sizeof(CMatrix));
}

CMatrix::~CMatrix()
{

}

CMatrix &CMatrix::operator*(CMatrix &right)
{
	static CMatrix tmp;
	memset(&tmp, 0, sizeof(CMatrix));
	for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
	for (int k = 0; k < 3; k++)
		tmp.m_fRows[i][j] += m_fRows[i][k] * right.m_fRows[k][j];
	return tmp;
}

CMatrix &CMatrix::operator*=(CMatrix &right)
{
	CMatrix tmp;
	memset(&tmp, 0, sizeof(CMatrix));
	for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
	for (int k = 0; k < 3; k++)
		tmp.m_fRows[i][j] += m_fRows[i][k] * right.m_fRows[k][j];
	memcpy(this, &tmp, sizeof(CMatrix));
	return *this;
}

CMatrix &CMatrix::operator=(CMatrix &right)
{
	memcpy(this, &right, sizeof(CMatrix));
	return *this;
}

void CMatrix::SetIdentity()
{
	memset(this, 0, sizeof(CMatrix));
	m_fRightX = 1.f;
	m_fTopY = 1.f;
	m_fAtZ = 1.f;
}

void CMatrix::RotateX(float x)
{
	x = x * PI / 180.f; // To radians
	SetIdentity();
	m_fTopY = cosf(x);
	m_fTopZ = sinf(x);
	m_fAtY = -sinf(x);
	m_fAtZ = cosf(x);
}

void CMatrix::RotateY(float y)
{
	y = y * PI / 180.f; // To radians
	SetIdentity();
	m_fRightX = cosf(y);
	m_fRightZ = sinf(y);
	m_fAtX = -sinf(y);
	m_fAtZ = cosf(y);
}

void CMatrix::RotateZ(float z)
{
	z = z * PI / 180.f; // To radians
	SetIdentity();
	m_fRightX = cosf(z);
	m_fRightY = sinf(z);
	m_fTopX = -sinf(z);
	m_fTopY = cosf(z);
}

void CMatrix::Rotate(float x, float y, float z)
{
	CMatrix tmp;
	memset(&tmp, 0, sizeof(CMatrix));
	SetIdentity();
	tmp.RotateX(x);
	*this *= tmp;
	tmp.RotateY(y);
	*this *= tmp;
	tmp.RotateZ(z);
	*this *= tmp;
}