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

#define LT2VERTFVF (FVF_XYZ | FVF_DIFFUSE | FVF_TEX2)


//----------------
// Vertex Structures
//----------------
// Desc: Each vertex structure is named according to how the vertex is to be used.
//       LVERTEX: An Untransformed, Lit (colored) vertex.
//		 TLVERTEX: Transformed and Lit
//		 TLTVERTEX: Transformed, Lit, and Textured Vertex
//		 LTVERTEX: Lit, Textured Vertex
//		 TNVERTEX: Textured Vertex with Normal

class LVERTEX
{
public:
	float x, y, z;
	unsigned long color;
	unsigned long type(){
		return LVERTFVF;}
};

class TLVERTEX
{
public:
	float x, y, z, rhw;
	unsigned long color;
	unsigned long type(){
		return TLVERTFVF;}
};

class TLTVERTEX
{
public:
	float x, y, z; 
	float rhw;
	unsigned long color;
	float tu, tv;
	unsigned long type(){
		return TLTVERTFVF;}
};

class LTVERTEX
{
public:
	float x;
	float y;
	float z;
	unsigned long color;
	float tu;
	float tv;
	unsigned long type(){
		return LTVERTFVF;}
};

class LT2VERTEX
{
public:
	float x;
	float y;
	float z;
	unsigned long color;
	float tu;
	float tv;
	float tu2;
	float tv2;
	unsigned long type(){
		return LT2VERTFVF;}
};

class TVERTEX
{
public:
	float x;
	float y;
	float z;
	
	float tu;
	float tv;
	unsigned long type(){
		return TVERTFVF;}
	
};

class TNVERTEX
{
public:
	float x;
	float y;
	float z;
	
	//unsigned long color;

	float nx;
	float ny;
	float nz;

	float tu;
	float tv;
	unsigned long type(){
		return TNVERTFVF;}
	
};

union Color32
{
	unsigned long color;
	unsigned char argb[4];
};

//----------------
// Vertex Creation Functions
// Desc: Creates a Vertex of the specified type.

inline TLTVERTEX TLTVertex(float x, float y, float z, float rhw, unsigned long color, float tu, float tv)
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

inline LVERTEX LVertex(float x, float y, float z, unsigned long color)
{

	LVERTEX temp;
	temp.x = x;
	temp.y = y; 
	temp.z = z;
	temp.color = color;
	
	return temp;
}

inline TLVERTEX TLVertex(float x, float y, float z, float rhw, unsigned long color)
{

	TLVERTEX temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	temp.rhw = rhw;
	temp.color = color;

	return temp;
}

inline LTVERTEX LTVertex(float x, float y, float z, unsigned long color, float tu, float tv)
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

inline LT2VERTEX LT2Vertex(float x, float y, float z, unsigned long color, float tu, float tv, float tu2, float tv2)
{

	LT2VERTEX temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	temp.color = color;
	temp.tu = tu;
	temp.tv = tv;
	temp.tu2 = tu2;
	temp.tv2 = tv2;

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

inline unsigned long ARGB(float a, float r, float g, float b)
{
	unsigned long f;
	char _a, _r, _g, _b;
	_a = (char)(0xff*a);
	_r = (char)(0xff*r);
	_g = (char)(0xff*g);
	_b = (char)(0xff*b);
	f  = _b;
	f |= _g << 8;
	f |= _r << 16;
	f |= _a << 24;
	return f;
}

inline unsigned long ColorInterpolate(unsigned long c1, unsigned long c2, float s)
{

	Color32 a, b;
	a.color = c1;
	b.color = c2;

	a.argb[0] = a.argb[0] + char(s * (b.argb[0] - a.argb[0]));
	a.argb[1] = a.argb[1] + char(s * (b.argb[1] - a.argb[1]));
	a.argb[2] = a.argb[2] + char(s * (b.argb[2] - a.argb[2]));
	a.argb[3] = a.argb[3] + char(s * (b.argb[3] - a.argb[3]));
	
	return a.color;
}

inline unsigned long s_to_color(const char* s)
{
	int len;
	unsigned long r = 0;
	char c;
	if(s[0] == '#')
	{
		// hex code
		len = strlen(s+1);
		for(int i = 0; i < len; ++i)
		{
			c = s[i+1];
			if((c >= '0') && (c <= '9'))
			{
				r += (c-'0') << (4*(len-i));
			}
			if(c >= 'a') && (c <= 'f'))
			{
				r += (c-'a'+10) << (4*(len-i));
			}
		}
	}
	return r;
}

#endif