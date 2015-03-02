#ifndef _FVF_H
#define _FVF_H

#define FVF_RESERVED0        0x001
#define FVF_POSITION_MASK    0x00E
#define FVF_XYZ              0x002
#define FVF_XYZRHW           0x004
#define FVF_XYZB1            0x006
#define FVF_XYZB2            0x008
#define FVF_XYZB3            0x00a
#define FVF_XYZB4            0x00c
#define FVF_XYZB5            0x00e

#define FVF_NORMAL           0x010
#define FVF_PSIZE            0x020
#define FVF_DIFFUSE          0x040
#define FVF_SPECULAR         0x080

#define FVF_TEXCOUNT_MASK    0xf00
#define FVF_TEXCOUNT_SHIFT   8
#define FVF_TEX0             0x000
#define FVF_TEX1             0x100
#define FVF_TEX2             0x200
#define FVF_TEX3             0x300
#define FVF_TEX4             0x400
#define FVF_TEX5             0x500
#define FVF_TEX6             0x600
#define FVF_TEX7             0x700
#define FVF_TEX8             0x800

#define FVF_LASTBETA_UBYTE4  0x1000

#define FVF_RESERVED2        0xE000


#define LOCK_READONLY           0x00000010L
#define LOCK_DISCARD             0x00002000L
#define LOCK_NOOVERWRITE        0x00001000L
#define LOCK_NOSYSLOCK          0x00000800L

#define LOCK_NO_DIRTY_UPDATE     0x00008000L

#define USAGE_WRITEONLY          (0x00000008L)
#define USAGE_SOFTWAREPROCESSING (0x00000010L)
#define USAGE_DONOTCLIP          (0x00000020L)
#define USAGE_POINTS             (0x00000040L)
#define USAGE_RTPATCHES          (0x00000080L)
#define USAGE_NPATCHES           (0x00000100L)
#define USAGE_DYNAMIC            (0x00000200L)

enum RENDERSTATETYPE {
    RS_ZENABLE                   = 7,    /* D3DZBUFFERTYPE (or TRUE/FALSE for legacy) */
    RS_FILLMODE                  = 8,    /* D3DFILL_MODE        */
    RS_SHADEMODE                 = 9,    /* D3DSHADEMODE */
    RS_LINEPATTERN               = 10,   /* D3DLINEPATTERN */
    RS_ZWRITEENABLE              = 14,   /* TRUE to enable z writes */
    RS_ALPHATESTENABLE           = 15,   /* TRUE to enable alpha tests */
    RS_LASTPIXEL                 = 16,   /* TRUE for last-pixel on lines */
    RS_SRCBLEND                  = 19,   /* D3DBLEND */
    RS_DESTBLEND                 = 20,   /* D3DBLEND */
    RS_CULLMODE                  = 22,   /* D3DCULL */
    RS_ZFUNC                     = 23,   /* D3DCMPFUNC */
    RS_ALPHAREF                  = 24,   /* D3DFIXED */
    RS_ALPHAFUNC                 = 25,   /* D3DCMPFUNC */
    RS_DITHERENABLE              = 26,   /* TRUE to enable dithering */
    RS_ALPHABLENDENABLE          = 27,   /* TRUE to enable alpha blending */
    RS_FOGENABLE                 = 28,   /* TRUE to enable fog blending */
    RS_SPECULARENABLE            = 29,   /* TRUE to enable specular */
    RS_ZVISIBLE                  = 30,   /* TRUE to enable z checking */
    RS_FOGCOLOR                  = 34,   /* D3DCOLOR */
    RS_FOGTABLEMODE              = 35,   /* D3DFOGMODE */
    RS_FOGSTART                  = 36,   /* Fog start (for both vertex and pixel fog) */
    RS_FOGEND                    = 37,   /* Fog end      */
    RS_FOGDENSITY                = 38,   /* Fog density  */
    RS_EDGEANTIALIAS             = 40,   /* TRUE to enable edge antialiasing */
    RS_ZBIAS                     = 47,   /* LONG Z bias */
    RS_RANGEFOGENABLE            = 48,   /* Enables range-based fog */
    RS_STENCILENABLE             = 52,   /* BOOL enable/disable stenciling */
    RS_STENCILFAIL               = 53,   /* D3DSTENCILOP to do if stencil test fails */
    RS_STENCILZFAIL              = 54,   /* D3DSTENCILOP to do if stencil test passes and Z test fails */
    RS_STENCILPASS               = 55,   /* D3DSTENCILOP to do if both stencil and Z tests pass */
    RS_STENCILFUNC               = 56,   /* D3DCMPFUNC fn.  Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
    RS_STENCILREF                = 57,   /* Reference value used in stencil test */
    RS_STENCILMASK               = 58,   /* Mask value used in stencil test */
    RS_STENCILWRITEMASK          = 59,   /* Write mask applied to values written to stencil buffer */
    RS_TEXTUREFACTOR             = 60,   /* D3DCOLOR used for multi-texture blend */
    RS_WRAP0                     = 128,  /* wrap for 1st texture coord. set */
    RS_WRAP1                     = 129,  /* wrap for 2nd texture coord. set */
    RS_WRAP2                     = 130,  /* wrap for 3rd texture coord. set */
    RS_WRAP3                     = 131,  /* wrap for 4th texture coord. set */
    RS_WRAP4                     = 132,  /* wrap for 5th texture coord. set */
    RS_WRAP5                     = 133,  /* wrap for 6th texture coord. set */
    RS_WRAP6                     = 134,  /* wrap for 7th texture coord. set */
    RS_WRAP7                     = 135,  /* wrap for 8th texture coord. set */
    RS_CLIPPING                  = 136,
    RS_LIGHTING                  = 137,
    RS_AMBIENT                   = 139,
    RS_FOGVERTEXMODE             = 140,
    RS_COLORVERTEX               = 141,
    RS_LOCALVIEWER               = 142,
    RS_NORMALIZENORMALS          = 143,
    RS_DIFFUSEMATERIALSOURCE     = 145,
    RS_SPECULARMATERIALSOURCE    = 146,
    RS_AMBIENTMATERIALSOURCE     = 147,
    RS_EMISSIVEMATERIALSOURCE    = 148,
    RS_VERTEXBLEND               = 151,
    RS_CLIPPLANEENABLE           = 152,
    RS_SOFTWAREVERTEXPROCESSING  = 153,
    RS_POINTSIZE                 = 154,   /* float point size */
    RS_POINTSIZE_MIN             = 155,   /* float point size min threshold */
    RS_POINTSPRITEENABLE         = 156,   /* BOOL point texture coord control */
    RS_POINTSCALEENABLE          = 157,   /* BOOL point size scale enable */
    RS_POINTSCALE_A              = 158,   /* float point attenuation A value */
    RS_POINTSCALE_B              = 159,   /* float point attenuation B value */
    RS_POINTSCALE_C              = 160,   /* float point attenuation C value */
    RS_MULTISAMPLEANTIALIAS      = 161,  // BOOL - set to do FSAA with multisample buffer
    RS_MULTISAMPLEMASK           = 162,  // DWORD - per-sample enable/disable
    RS_PATCHEDGESTYLE            = 163,  // Sets whether patch edges will use float style tessellation
    RS_PATCHSEGMENTS             = 164,  // Number of segments per edge when drawing patches
    RS_DEBUGMONITORTOKEN         = 165,  // DEBUG ONLY - token to debug monitor
    RS_POINTSIZE_MAX             = 166,   /* float point size max threshold */
    RS_INDEXEDVERTEXBLENDENABLE  = 167,
    RS_COLORWRITEENABLE          = 168,  // per-channel write enable
    RS_TWEENFACTOR               = 170,   // float tween factor
    RS_BLENDOP                   = 171,   // D3DBLENDOP setting

    RS_FORCE_DWORD               = 0x7fffffff, /* force 32-bit size enum */
};

enum SHADEMODE {
    SHADE_FLAT               = 1,
    SHADE_GOURAUD            = 2,
    SHADE_PHONG              = 3,
    SHADE_FORCE_DWORD        = 0x7fffffff, /* force 32-bit size enum */
};

enum FILLMODE {
    FILL_POINT               = 1,
    FILL_WIREFRAME           = 2,
    FILL_SOLID               = 3,
    FILL_FORCE_DWORD         = 0x7fffffff, /* force 32-bit size enum */
};

struct LINEPATTERN {
    WORD    wRepeatFactor;
    WORD    wLinePattern;
};

enum BLEND {
    BLEND_ZERO               = 1,
    BLEND_ONE                = 2,
    BLEND_SRCCOLOR           = 3,
    BLEND_INVSRCCOLOR        = 4,
    BLEND_SRCALPHA           = 5,
    BLEND_INVSRCALPHA        = 6,
    BLEND_DESTALPHA          = 7,
    BLEND_INVDESTALPHA       = 8,
    BLEND_DESTCOLOR          = 9,
    BLEND_INVDESTCOLOR       = 10,
    BLEND_SRCALPHASAT        = 11,
    BLEND_BOTHSRCALPHA       = 12,
    BLEND_BOTHINVSRCALPHA    = 13,
    BLEND_FORCE_DWORD        = 0x7fffffff, /* force 32-bit size enum */
};

enum BLENDOP {
    BLENDOP_ADD              = 1,
    BLENDOP_SUBTRACT         = 2,
    BLENDOP_REVSUBTRACT      = 3,
    BLENDOP_MIN              = 4,
    BLENDOP_MAX              = 5,
    BLENDOP_FORCE_DWORD      = 0x7fffffff, /* force 32-bit size enum */
};

enum TEXTUREADDRESS {
    TADDRESS_WRAP            = 1,
    TADDRESS_MIRROR          = 2,
    TADDRESS_CLAMP           = 3,
    TADDRESS_BORDER          = 4,
    TADDRESS_MIRRORONCE      = 5,
    TADDRESS_FORCE_DWORD     = 0x7fffffff, /* force 32-bit size enum */
};

enum CULL {
    CULL_NONE                = 1,
    CULL_CW                  = 2,
    CULL_CCW                 = 3,
    CULL_FORCE_DWORD         = 0x7fffffff, /* force 32-bit size enum */
};

enum CMPFUNC {
    CMP_NEVER                = 1,
    CMP_LESS                 = 2,
    CMP_EQUAL                = 3,
    CMP_LESSEQUAL            = 4,
    CMP_GREATER              = 5,
    CMP_NOTEQUAL             = 6,
    CMP_GREATEREQUAL         = 7,
    CMP_ALWAYS               = 8,
    CMP_FORCE_DWORD          = 0x7fffffff, /* force 32-bit size enum */
};

enum STENCILOP {
    STENCILOP_KEEP           = 1,
    STENCILOP_ZERO           = 2,
    STENCILOP_REPLACE        = 3,
    STENCILOP_INCRSAT        = 4,
    STENCILOP_DECRSAT        = 5,
    STENCILOP_INVERT         = 6,
    STENCILOP_INCR           = 7,
    STENCILOP_DECR           = 8,
    STENCILOP_FORCE_DWORD    = 0x7fffffff, /* force 32-bit size enum */
};

enum FOGMODE {
    FOG_NONE                 = 0,
    FOG_EXP                  = 1,
    FOG_EXP2                 = 2,
    FOG_LINEAR               = 3,
    FOG_FORCE_DWORD          = 0x7fffffff, /* force 32-bit size enum */
};

enum ZBUFFERTYPE {
    ZB_FALSE                 = 0,
    ZB_TRUE                  = 1, // Z buffering
    ZB_USEW                  = 2, // W buffering
    ZB_FORCE_DWORD           = 0x7fffffff, /* force 32-bit size enum */
};

struct COLORVALUE {
	float r;
	float g;
	float b;
	float a;
};

struct MATERIAL {
    COLORVALUE   Diffuse;        /* Diffuse color RGBA */
    COLORVALUE   Ambient;        /* Ambient color RGB */
    COLORVALUE   Specular;       /* Specular 'shininess' */
    COLORVALUE   Emissive;       /* Emissive color RGB */
    float        Power;          /* Sharpness if specular highlight */
};

struct PLANE {
	float a;
	float b;
	float c;
	float d;
};
#endif