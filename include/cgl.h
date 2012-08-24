#ifndef cgl_h__
#define cgl_h__

#pragma warning (disable : 4251)
#define _CRT_SECURE_NO_WARNINGS

#ifdef CLEARGRAPHICSLIBRARY_EXPORTS
#define CGL_API __declspec(dllexport)
#else
#define CGL_API __declspec(dllimport)
#endif

#define SAFE_RELEASE(x)			if(x){x->Release(); x = NULL;}
#define SAFE_DELETE(x)			if(x){delete x; x = NULL;}
#define SAFE_DELETE_ARRAY(x)	if(x){delete[] x; x = NULL;}
#define SAFE_FREE(x)			if(x){free(x); x = NULL;}

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WindowsX.h>

#include <stdlib.h>
#include <string>
#include <memory>
#include <list>
#include <sstream>
#include <vector>
#include <assert.h>
#include <map>
#include <iomanip>
#include <algorithm>

#include <d3d11.h>
#include <DXGI.h>	
#include <D3DX11.h>
#include <d3dx11effect.h>
#include <DxErr.h>
#include <D3Dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "effects11.lib")

#include "CGLObject.h"
#include "CGLInstancing.h"
#include "CGLDataProvider.h"
#include "CGLInputProcessor.h"
#include "CGLLogger.h"
#include "CGLWindow.h"
#include "CGLGameLoop.h"

#include "DXGIFactory.h"
#include "D3D11Device.h"
#include "D3D11Effect.h"
#include "D3D11InputLayout.h"
#include "D3D11Buffer.h"
#include "D3D11InputBuffer.h"
#include "D3D11RenderStates.h"
#include "D3D11Texture.h"
#include "D3D11ResourceView.h"
#include "D3D11Query.h"

#include "CGLBindCollection.h"

#include "CGLManager.h"

#endif // cgl_h__
