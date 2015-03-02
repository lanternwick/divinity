#include "TOW_HeightMap.h"
#include "..\DivCore\Core.h"


TOW_HeightMap::TOW_HeightMap()
{
	map = NULL;
}

TOW_HeightMap::~TOW_HeightMap()
{
	if(map) delete map;
	map = NULL;
}

float TOW_HeightMap::GetHeight(float x, float z)
{
	/*float total = 0;
	int p = 2;
	int n = 1;
	float frequency;
	float amplitude;

	for(int i = 1; i < n+1; ++i)
	{
		frequency = (float)pow(2.0f, float(i));
		amplitude = (float)pow(float(p), float(i));

		total += m_intNoise(x * frequency, z * frequency) * amplitude;
	}*/
	if((x < width) && (z < width) && (x > 0.0f) && (z > 0.0f))
		return m_intNoise(x, z);
	return 0.0f;

}

float TOW_HeightMap::GetRawHeight(int x, int z)
{
	if((x < width) && (z < width) && (x > 0.0f) && (z > 0.0f)) 
		return float(map[x+z*width]) / 13.5f;
	return 0.0f;
}

float TOW_HeightMap::m_noise(int x, int y)
{
	
    long n = (long)pow((float)x, (float)y);
	//n = (long)pow((float)n, 1.0f/(float)x);

    return float( 1.0 - ( (n * (y * (x-n) * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float TOW_HeightMap::m_smoothNoise(float x, float y)
{
	float corners = ( m_noise((int)x-1, (int)y-1)+m_noise((int)x+1, (int)y-1)+m_noise((int)x-1, (int)y+1)+m_noise((int)x+1, (int)y+1) ) / 16;
    float sides   = ( m_noise((int)x-1, (int)y)  +m_noise((int)x+1, (int)y)  +m_noise((int)x, (int)y-1)  +m_noise((int)x, (int)y+1) ) /  8;
    float center  =  m_noise((int)x, (int)y) / 4;
    return corners + sides + center;

}

float TOW_HeightMap::m_cosInterpolate(float a, float b, float s)
{
	float ft = s * 3.1415927f;
	float f = float(1.0 - cos(ft)) * 0.5f;

	return  a*(1.0f-f) + b*f;

}

float TOW_HeightMap::m_intNoise(float x, float y)
{
	int integer_X    = int(x);
	float fractional_X = x - integer_X;

	int integer_Y    = int(y);
	float fractional_Y = y - integer_Y;

	
	float v1, v2, v3, v4;
	v1 = GetRawHeight(integer_X, integer_Y);
	v2 = GetRawHeight(integer_X+1, integer_Y);
	v3 = GetRawHeight(integer_X+1, integer_Y+1);
	v4 = GetRawHeight(integer_X, integer_Y+1);

	// float(map[integer_X+integer_Y*width]) / 13.5f;
	float i1 = m_cosInterpolate(v1 , v2 , fractional_X);
	float i2 = m_cosInterpolate(v3 , v4 , fractional_X);

	return m_cosInterpolate(i1 , i2 , fractional_Y);

}

void TOW_HeightMap::CreateMap(unsigned long w, unsigned char* buffer)
{
	width = w;
	map = buffer;
	return;
}

bool TOW_HeightMap::RayCast(VECTOR* v, VECTOR* src, VECTOR* dir, float detail)
{
	PLANE p;
	VECTOR t;
	VECTOR pts[3];
	VectorNormalize(dir, dir);
	for(int i = 0; i < 78; ++i)
	{
		for(int j = 0; j < 78; ++j)
		{
			// first triangle from heightmap
			pts[0] = VECTOR(float(i), GetHeight(float(i), float(j)), float(j));
			pts[1] = VECTOR(float(i+1), GetHeight(float(i+1), float(j)), float(j));
			pts[2] = VECTOR(float(i+1), GetHeight(float(i+1), float(j+1)), float(j+1));
			PlaneFromPoints(&p, &pts[0], &pts[1], &pts[2]);
			if(RayIntersectPlane(&t, &p, src, dir))
			{
				if(VertexInTriangle(&t, &pts[0], &pts[1], &pts[2]))
				{
					*v = t;
					return true;
				}
			}
			// second triangle from heightmap
			pts[0] = VECTOR(float(i), GetHeight(float(i), float(j)), float(j));
			pts[1] = VECTOR(float(i), GetHeight(float(i), float(j+1)), float(j+1));
			pts[2] = VECTOR(float(i+1), GetHeight(float(i+1), float(j+1)), float(j+1));
			PlaneFromPoints(&p, &pts[0], &pts[1], &pts[2]);
			if(RayIntersectPlane(&t, &p, src, dir))
			{
				if(VertexInTriangle(&t, &pts[0], &pts[1], &pts[2]))
				{
					*v = t;
					return true;
				}
			}
		}
	}


	return false;
}

bool TOW_HeightMap::Occluded(VECTOR* iv, VECTOR* src, VECTOR* dir)
{
	int w,hits;
  float d,h,D;
  VECTOR v,di;

  v = *iv + *dir;
  w = 78;

  hits = 0;
	
  while (!(( v.x >= w-1 ) || ( v.x <= 0 ) || ( v.z >= w-1 ) || ( v.z <= 0 ))){
    // length of lightdir's projection
    D = VectorMagnitude(&(VECTOR(v.x,0.0f,v.z)-VECTOR(src->x,0.0f,src->z)));
    d = VectorMagnitude(&(*iv-v));            // light direction
    h = iv->y + (d*src->y) / D;  // X(P) point
	//h -= 0.4f;
    // check if height in point P is bigger than point X's height
    if (GetHeight(v.x, v.z) > h){
      hits++;   // if so, mark as hit, and skip this work point.
      break;
    };

    di = *dir;
    di.y = 0.0f;
    VectorNormalize(&di, &di);   // fetch new working point
	v += di;
  };
  return hits > 0;
}