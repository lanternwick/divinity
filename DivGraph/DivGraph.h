
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DIVGRAPH_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DIVGRAPH_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.


#include "Graphics.h"
#include "CFrame.h"
#include "CScene.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "..\Common\CSmartPtr.h"

#include "ITexture.h"

#include "UserInterface.h"