#ifndef _TYPES_H
#define _TYPES_H

#define LVERTFVF (FVF_XYZ | FVF_DIFFUSE)
#define TLVERTFVF (FVF_XYZRHW | FVF_DIFFUSE)
#define TLTVERTFVF (FVF_XYZRHW | FVF_DIFFUSE | FVF_TEX1)

#define WINDOWVERTEX TLTVERTEX
#define WINDOWVERTFVF TLTVERTFVF
#define WindowVertex TLTVertex

#define LTVERTFVF (FVF_XYZ | FVF_DIFFUSE | FVF_TEX1)

#define MAPVERTFVF LTVERTFVF
#define MAPVERTEX LTVERTEX
#define MapVertex LTVertex

#define TVERTFVF (FVF_XYZ | FVF_TEX1)

#define TNVERTFVF (FVF_XYZ  | FVF_NORMAL | FVF_TEX1)


//----------------
// Vertex Structures
//----------------
// Desc: Each vertex structure is named according to how the vertex is to be used.
//       LVERTEX: An Untransformed, Lit (colored) vertex.
//		 TLVERTEX: Transformed and Lit
//		 TLTVERTEX: Transformed, Lit, and Textured Vertex
//		 LTVERTEX: Lit, Textured Vertex
//		 TNVERTEX: Textured Vertex with Normal

struct LVERTEX
{
	float x, y, z;
	DWORD color;
};

struct TLVERTEX
{
	float x, y, z, rhw;
	DWORD color;
};

struct TLTVERTEX
{
	float x, y, z; 
	float rhw;
	DWORD color;
	float tu, tv;
};

struct LTVERTEX
{
	float x;
	float y;
	float z;
	DWORD color;
	float tu;
	float tv;
};

struct TVERTEX
{
	float x;
	float y;
	float z;
	
	float tu;
	float tv;
	
};

struct TNVERTEX
{
	float x;
	float y;
	float z;
	
	//DWORD color;

	float nx;
	float ny;
	float nz;

	float tu;
	float tv;
	
};

//----------------
// Vertex Creation Functions
// Desc: Creates a Vertex of the specified type.

inline TLTVERTEX TLTVertex(float x, float y, float z, float rhw, DWORD color, float tu, float tv)
{
	TLTVERTEX temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	temp.rhw = rhw;
	temp.color = color;
	temp.tu = tu;
	temp.tv = tv;
	
	return temp;
}

inline LVERTEX LVertex(float x, float y, float z, DWORD color)
{
	LVERTEX temp;
	temp.x = x;
	temp.y = y; 
	temp.z = z;
	temp.color = color;
	
	return temp;
}

inline TLVERTEX TLVertex(float x, float y, float z, float rhw, DWORD color)
{
	TLVERTEX temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	temp.rhw = rhw;
	temp.color = color;

	return temp;
}

inline LTVERTEX LTVertex(float x, float y, float z, DWORD color, float tu, float tv)
{
	LTVERTEX temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	temp.color = color;
	temp.tu = tu;
	temp.tv = tv;

	return temp;
}

inline TVERTEX TVertex(float x, float y, float z, float tu, float tv)
{
	TVERTEX temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	
	temp.tu = tu;
	temp.tv = tv;
	return temp;
}

inline TNVERTEX TNVertex(float x, float y, float z, float nx, float ny, float nz, float tu, float tv)
{
	TNVERTEX temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	
	//temp.color = color;

	temp.tu = tu;
	temp.tv = tv;
	temp.nx = 0.0f;
	temp.ny = 1.0f;
	temp.nz = 0.0f;
	return temp;
}

#endif