#ifndef DEBUG_H
#define DEBUG_H

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
	#define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__)
	#define MEM_LEAK_TRACING() int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;  _CrtSetDbgFlag(tmpDbgFlag)
#else
	#define DEBUG_CLIENTBLOCK new
	#define MEM_LEAK_TRACING()
#endif



#endif