#ifndef CVECTOR_CLASS_H
#define CVECTOR_CLASS_H

class CVector
{
public:
	float x;
	float y;
	float z;

	CVector();
	float Length();
	void Normalize();
};

#endif