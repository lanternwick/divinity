#include "TOW_Map.h"
extern "C" {
	#include <lua.h>
}
#include "lua_glue.h"

TOW_Map::TOW_Map()
{
	Core::Function_Enter("TOW_Map::Constuctor()");
	m_heightMap = NULL;
	m_textureMap = NULL;
	
	
	tiles = NULL;
	m_re_calc_culling = true;
	m_id = "global_map";
	map_cx = 0;
	map_cz = 0;
	m_tile_size = 0.0f;
	
	
	Core::Function_Exit();
}

TOW_Map::~TOW_Map()
{
	Core::Function_Enter("TOW_Map::Destuctor()");
	Destroy();
	
	
	Core::Function_Exit();
}

IHeightMap* TOW_Map::HeightMap()
{
	Core::Function_Enter("TOW_Map::HeightMap()");
	
	IHeightMap* i = m_heightMap;
	Core::Function_Exit();
	return m_heightMap;
}

ITextureMap* TOW_Map::TextureMap()
{
	Core::Function_Enter("TOW_Map::TextureMap()");
	ITextureMap* i = m_textureMap;
	Core::Function_Exit();
	return m_textureMap;
}

RESULT TOW_Map::Load(char* file)
{
	m_allow.lock();
	C_FILE* cf;
	FC_FILE* fcf;
	cf = c_open(file);
	fcf = fc_open(cf, "heightmap");
	unsigned long w;
	fc_read(&w, 1, 4, fcf);
	unsigned char* buffer = new unsigned char[w*w];
	fc_read(buffer, 1, w*w, fcf);
	m_heightMap = new TOW_HeightMap;
	// object USES buffer, does NOT copy it (!!!!!!!UNSAFE!!!!!!)
	m_heightMap->CreateMap(w, buffer);
	
	fc_close(fcf);
	m_textureMap = new TOW_TextureMap;
	fcf = fc_open(cf, "texturemap");
	fc_read(&w, 1, 4, fcf);
	buffer = new unsigned char[w*w];
	fc_read(buffer, 1, w*w, fcf);
	fc_close(fcf);
	m_textureMap->CreateMap(w, buffer);
	
	fcf = fc_open(cf, "init");
	if(fcf) // init script exists...
	{
		char* lua_buffer;
		size_t size;
		fc_seek(fcf, 0, SEEK_END);
		size = fc_tell(fcf);
		fc_seek(fcf, 0, SEEK_SET);
		lua_buffer = new char[size];
		fc_read(lua_buffer, 1, size, fcf);
		fc_close(fcf);
		lua_lua_dobuffer(lua_buffer, size);
		delete lua_buffer;
	}
	
	
	//int x, y;
	//float size;
	fcf = fc_open(cf, "map_attributes");
	if(fcf)
	{
		std::string s;
		char c = 0;
		while(true)		
		{
			fc_read(&c, 1, 1, fcf);
			if(c == '\n')
			{
				break;
			}
			s += c;
		}
		map_cx = atoi(s.c_str());
		s.erase();
		while(true)		
		{
			fc_read(&c, 1, 1, fcf);
			if(c == '\n')
			{
				break;
			}
			s += c;
		}
		map_cz = atoi(s.c_str());
		s.erase();
		while(true)		
		{
			fc_read(&c, 1, 1, fcf);
			if(c == '\n')
			{
				break;
			}
			s += c;
		}
		m_tile_size = atof(s.c_str());


	}
	fc_close(fcf);
	m_file = file;
	c_close(cf);
	m_allow.unlock();
	return OK;
}

void TOW_Map::SetDevice(IDevice* device)
{
	myDevice = device;
}

RESULT TOW_Map::Create(int x, int z, float size)
{
	m_allow.lock();
	tiles = new Terrain_Tile[x*z];
	if(!tiles)
		Core::WriteLog("Terrain Creation Failed!\n", 1);
	Terrain_Tile* tt = tiles;
	float curr_x = 0.0f;
	float curr_z = 0.0f;
	float temp = size;
	int c = 0;
	if(map_cx != 0)
	{
		map_cx = x;
		map_cz = z;
		m_tile_size = size;
	}
	for(int i = 0; i < map_cx; ++i)
	{
		for(int j = 0; j < map_cz; ++j)
		{
			tiles[c].Create(curr_x, curr_z, size, (int)size);
			tiles[c].SetTileID(c);
			curr_x += temp;
			
			//m_textureMap->AttachTile(c++, rand() % 2 ? "grass" : "rock");
			++c;
		}
		curr_z += temp;
		curr_x = 0.0f;
	}
	
	m_allow.unlock();
	return OK;
}

RESULT TOW_Map::Destroy()
{
	m_allow.lock();
	C_FILE* cf;
	FC_FILE* fcf;
	cf = c_open((char*)m_file.c_str());
	fcf = fc_open(cf, "term");
	if(fcf) // term script exists...
	{
		char* lua_buffer;
		size_t size;
		fc_seek(fcf, 0, SEEK_END);
		size = fc_tell(fcf);
		fc_seek(fcf, 0, SEEK_SET);
		lua_buffer = new char[size];
		fc_read(lua_buffer, 1, size, fcf);

		lua_lua_dobuffer(lua_buffer, size);
		delete lua_buffer;
	}
	c_close(cf);
	if(m_heightMap)
		delete m_heightMap;
	m_heightMap = NULL;
	if(m_textureMap)
		delete m_textureMap;
	m_textureMap = NULL;
	if(tiles)
		delete [] tiles;
	tiles = NULL;
	map_cx = 0;
	map_cz = 0;
	m_tile_size = 0.0f;
	m_allow.unlock();
	return OK;
}

void TOW_Map::ToggleCulling()
{
	m_re_calc_culling = !m_re_calc_culling;
}

RESULT TOW_Map::Render()
{
	int c = 0;
	CCamera cam;
	m_scene->GetCamera(&cam);
	int x;
	int y;
	m_allow.lock();
	

	//vector<vector<Terrain_Tile*> > sort_Tiles;
	//sort_Tiles.resize(m_textureMap->GetNumTextures());
	/*for(int i = 0; i < m_textureMap->GetNumTextures(); ++i)
	{
		((ITexture*)Core::GetMediaObject(m_textureMap->GetTexture(i)))->Set(i);
	}*/
	/*myDevice->SetTextureState(0, TS_COLOROP, TOP_MODULATE);
	myDevice->SetTextureState(0, TS_COLORARG1, TA_TEXTURE);
	myDevice->SetTextureState(0, TS_COLORARG2, TA_DIFFUSE);
	//myDevice->SetTextureState(1, TS_COLOROP, TOP_BLENDDIFFUSEALPHA);
	myDevice->SetTextureState(0, TS_ALPHAOP, TOP_SELECTARG1);
	myDevice->SetTextureState(0, TS_ALPHAARG1, TA_TEXTURE);
	myDevice->SetTextureState(0, TS_MAGFILTER, 2);
	myDevice->SetTextureState(1, TS_MAGFILTER, 2);*/
	//myDevice->SetTextureState(2, TS_COLOROP, TOP_MODULATE);
	//myDevice->SetTextureState(2, TS_COLORARG1, TA_CURRENT);
	//myDevice->SetTextureState(2, TS_COLORARG2, TA_DIFFUSE);
	
	//	Call D3DDevice.SetTextureStageState(1, D3DTSS_COLORARG0, D3DTA_SPECULAR)
     //   Call D3DDevice.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDDIFFUSEALPHA)
      //  Call D3DDevice.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE)
       // Call D3DDevice.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT)
        //Call D3DDevice.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE)
	MATRIX* m = new MATRIX;
	MatrixMatrixIdentity(&m);
	//int c = 0;
	
	if(m_re_calc_culling)
	{
		CalcCullRect(&cam, &m_cull_rect);
	}	
	
	//Core::WriteLog(buff);
	if(tiles)
	{
		if(true)//m_tile_size > cam.vecPos.y)
		{
			x = cam.vecPos.x/m_tile_size;
			y = cam.vecPos.z/m_tile_size;
			
			//for(int i = x-1; i < x+cam.vecPos.y; ++i)
			for(int i = m_cull_rect.left; i < m_cull_rect.right+1; ++i)
			{
				//for(int j = y-1; j < y+cam.vecPos.y; ++j)
				for(int j = m_cull_rect.bottom; j < m_cull_rect.top +1; ++j)
				{
					//m_textureMap->GetTextureByTile(tiles[i*map_cx+j].GetTileID())->Set(0);
					//ITexture::MakeNull(m_device);
					//sort_Tiles[tiles[i*map_cx+j].GetTileID()].push_back(&tiles[i*map_cx+j]);
					if((j >= map_cz) || (j < 0)) continue;
					if((i >= map_cx) || (i < 0)) continue;
					myDevice->SetTransform(T_WORLD, m);
					/*for(int k = 0; k < m_textureMap->GetNumTextures(); ++k)
					{
						((ITexture*)Core::GetMediaObject(m_textureMap->GetTexture(k)))->Set(k);		
					}
					myDevice->SetTextureState(0, TS_COLOROP, TOP_SELECTARG1);
					myDevice->SetTextureState(0, TS_COLORARG1, TA_TEXTURE);
					myDevice->SetTextureState(0, TS_COLORARG2, TA_DIFFUSE);
					myDevice->SetTextureState(1, TS_COLOROP, TOP_BLENDDIFFUSEALPHA);
					myDevice->SetTextureState(0, TS_ALPHAOP, TOP_SELECTARG1);
					myDevice->SetTextureState(0, TS_ALPHAARG1, TA_TEXTURE);*/
					tiles[j*map_cz+i].Render();
					++c;
				}
			}
		}
		else
		{
			/*for(int i = 0; i < map_cx; ++i)
			{
				for(int j = 0; j < map_cz; ++j)
				{
					if(fabs(sqrt(((float(i)*m_tile_size)*(float(i)*m_tile_size))+((float(j)*m_tile_size)*(float(j)*m_tile_size)))) < cam.vecPos.y*cam.vecPos.y)
					{
						m_textureMap->GetTextureByTile(tiles[c].GetTileID())->Set();
						tiles[c++].Render();
					}
				}
			}*/
		}
	}
	/*for(int i = 0; i < sort_Tiles.size(); ++i)
	{
		m_textureMap->GetTextureByTile(i)->Set();
		for(int j = 0; j < sort_Tiles[i].size(); ++j)
		{
			sort_Tiles[i][j]->Render();
		}
	}*/
	TOW_ObjectManager::Render();
	delete m;
	m_allow.unlock();
	return OK;
}

void TOW_Map::SetScene(CScene* scene)
{
	Core::Function_Enter("TOW_Map::SetScene()");
	m_scene = scene;
	Core::Function_Exit();
}

void TOW_Map::CalcCullRect(CCamera* cam, RECT* r)
{
	ray bb[4];
	PLANE p;
	VECTOR out[4];
	bool results[4];

	myDevice->ComputeRay(&bb[0], &VECTOR(0, 0, 0), cam);
	myDevice->ComputeRay(&bb[1], &VECTOR(myDevice->width(), 0, 0), cam);
	//myDevice->ComputeRay(&bb[2], &VECTOR(0, myDevice->height(), 0), cam);
	myDevice->ComputeRay(&bb[3], &VECTOR(myDevice->width(), myDevice->height(), 0), cam);

	p.a = 0;
	p.b = 1;
	p.c = 0;
	p.d = 0;

	results[0] = bb[0].intersect_plane(&out[0], &p);
	results[1] = bb[1].intersect_plane(&out[1], &p);
	
	results[3] = bb[3].intersect_plane(&out[3], &p);

	//float left, right, top, bottom;

	r->left = out[0].x / m_tile_size;
	
	r->right = out[1].x / m_tile_size;
	if(results[3])
		r->top = out[0].z / m_tile_size;
	else
		m_cull_rect.top = map_cz;
	r->bottom = out[3].z / m_tile_size;
	//if(out[0].x < 0.0f) return false;
	//if(out[1].x > m_tile_size * map_cx) return false;
	return;

}

bool TOW_Map::ViewWithinBounds(CCamera* cam)
{
	ray bb[4];
	PLANE p;
	VECTOR out[4];
	bool results[4];

	myDevice->ComputeRay(&bb[0], &VECTOR(0, 0, 0), cam);
	myDevice->ComputeRay(&bb[1], &VECTOR(myDevice->width(), 0, 0), cam);
	myDevice->ComputeRay(&bb[2], &VECTOR(0, myDevice->height(), 0), cam);
	myDevice->ComputeRay(&bb[3], &VECTOR(myDevice->width(), myDevice->height(), 0), cam);

	p.a = 0;
	p.b = 1;
	p.c = 0;
	p.d = 0;

	results[0] = bb[0].intersect_plane(&out[0], &p);
	results[1] = bb[1].intersect_plane(&out[1], &p);
	results[1] = bb[2].intersect_plane(&out[2], &p);	
	results[3] = bb[3].intersect_plane(&out[3], &p);

	if(!((out[0].x >= 0.0f) &&
		(out[0].x < float(map_cx)*m_tile_size) &&
		(out[0].z >= 0.0f)  &&
		(out[0].z < float(map_cz)*m_tile_size)))
	{
		return false;
	}
	if(!((out[1].x >= 0.0f) &&
		(out[1].x < float(map_cx)*m_tile_size) &&
		(out[1].z >= 0.0f)  &&
		(out[1].z < float(map_cz)*m_tile_size)))
	{
		return false;
	}
	if(!results[3]) return false;
	if(!((out[3].x >= 0.0f) &&
		(out[3].x < float(map_cx)*m_tile_size) &&
		(out[3].z >= 0.0f)  &&
		(out[3].z < float(map_cz)*m_tile_size)))
	{
		return false;
	}
	if(!((out[2].x >= 0.0f) &&
		(out[2].x < float(map_cx)*m_tile_size) &&
		(out[2].z >= 0.0f)  &&
		(out[2].z < float(map_cz)*m_tile_size)))
	{
		return false;
	}
	return true;
}

int TOW_Map::width()
{
	return map_cx*m_tile_size;
}

int TOW_Map::height()
{
	return map_cz*m_tile_size;
}