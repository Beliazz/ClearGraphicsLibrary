#include "cgl.h"

cgl::core::PCGLManager cgl::CGLCreate( bool debug )
{
	if (debug)
	{
		return cgl::debug::CGLManagerDbg::Create();
	}
	else
	{
		return cgl::core::CGLManager::Create();
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
	                   DWORD  ul_reason_for_call,
	                   LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}
