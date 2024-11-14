#ifndef CVERTEX_CLASS_H
#define CVERTEX_CLASS_H

struct CVertex 
{
	float	x, y, z;
	float   nx, ny, nz;
	DWORD	color;
	float	tx, ty;
};

#define CVERTEX_TYPE (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#endif