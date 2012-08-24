#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// CGLLogger
bool cgl::CGLLogger::Print( LPCSTR log, ... )
{
	#ifdef _DEBUG // only output messages when debugging

	static CHAR buffer[CGL_LOG_BUFFER];		
	static SYSTEMTIME time;

	if (std::string(log).size() == 0)
	{	
		return true;
	}
	
	if (log == "\n")
	{
		//////////////////////////////////////////////////////////////////////////
		// post blank line
		_RPT0(0, "\n");

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// get arguments
	va_list pArgList;						
	if (sizeof(va_list) + sizeof(log) > CGL_LOG_BUFFER)
	{	
		return false;
	}

	va_start (pArgList, log);	
	vsprintf_s(buffer, log, pArgList);
	va_end (pArgList);

	//////////////////////////////////////////////////////////////////////////
	// build log msg
	char msg[CGL_LOG_BUFFER];
	#ifdef TIME_STAMP
	GetSystemTime(&time);
	sprintf_s(msg, "CGL - %02i:%02i:%02i.%03i - %s\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, buffer);
	#else
	sprintf_s(msg, "CGL - %s\n", buffer);
	#endif

	//////////////////////////////////////////////////////////////////////////
	// post message to the output window
	_RPT0(0, msg);

	//////////////////////////////////////////////////////////////////////////
	// reset static data
	ZeroMemory(&msg[0], sizeof(CGL_LOG_BUFFER));
	ZeroMemory(&time, sizeof(time));

	#endif // _DEBUG

	return true;
}
void cgl::CGLLogger::LogObjectState( UINT logType, cgl::CGLObject* pObject, HRESULT result, void* pData )
{
	switch (logType)
	{
	case CGL_NOTIFICATION_INVALID_PTR:			{ Print("ERROR: invalid ptr"); } break;
	case CGL_NOTIFICATION_INSTANTIATION:		{ Print("INFO:  instantiated name=\"%s\" type=\"%s\"",					pObject->getName().c_str(), pObject->getTypeName().c_str()); } break;
	case CGL_NOTIFICATION_CYCLIC_DEPENDENCY:	{ Print("ERROR: cyclic dependency detected name=\"%s\" type=\"%s\"",	pObject->getName().c_str(), pObject->getTypeName().c_str()); } break;
	case CGL_NOTIFICATION_RESET:				{ Print("INFO:  reset name=\"%s\" type=\"%s\"",							pObject->getName().c_str(), pObject->getTypeName().c_str()); } break;
	case CGL_NOTIFICATION_DESCTRUCTION:			{ Print("INFO:  destroyed name=\"%s\" type=\"%s\"",						pObject->getName().c_str(), pObject->getTypeName().c_str()); } break;
	case CGL_NOTIFICATION_STILL_ALIVE:			{ Print("WARNING: still alive name=\"%s\" type=\"%s\"",					pObject->getName().c_str(), pObject->getTypeName().c_str()); } break;
	case CGL_NOTIFICATION_COM_INTERFACE_STILL_ALIVE: { Print("ERROR:  name=\"%s\" type=\"%s\" -> OnReset() COM interface not fully released (%i refs)", pObject->getName().c_str(), pObject->getTypeName().c_str(), *((int*)pData) ); } break;
	case CGL_NOTIFICATION_NO_DEVICE:
		{
			if (pObject)
			{
				Print("ERROR: registration failed name=\"%s\" type=\"%s\"\n                            Error: no device registered!\n",
					pObject->getName().c_str(),	
					pObject->getTypeName().c_str());
			}
			else
			{
				Print("ERROR: no device registered!");
			}
		} break;

	case CGL_NOTIFICATION_CREATION:
		{
			if (SUCCEEDED(result))
			{
				Print("INFO:  created name=\"%s\" type=\"%s\"",
					  pObject->getName().c_str(),
					  pObject->getTypeName().c_str());
			}
			else
			{			
				Print("ERROR: creation failed name=\"%s\" type=\"%s\"\n                            Error: %s"
																	"\n                            Description: %s\n",
					 pObject->getName().c_str(),
					 pObject->getTypeName().c_str(),
					 DXGetErrorStringA(result),
					 DXGetErrorDescriptionA(result));
			}
		} break;

	case CGL_NOTIFICATION_RESTORATION:
		{
			if (SUCCEEDED(result))
			{
				Print("INFO:  restored name=\"%s\" type=\"%s\"",
					   pObject->getName().c_str(),
					   pObject->getTypeName().c_str());
			}
			else
			{
				Print("ERROR: failed name=\"%s\" type=\"%s\" -> OnRestore()\n                            Error: %s"
													                           "\n                            Description: %s\n",
					   pObject->getName().c_str(),
					   pObject->getTypeName().c_str(),
					   DXGetErrorStringA(result),
					   DXGetErrorDescriptionA(result));
			}
		} break;

	case CGL_NOTIFICATION_REGISTRATION:
		{
			if (SUCCEEDED(result))
			{
				Print("INFO:  registered name=\"%s\" type=\"%s\"",
					  pObject->getName().c_str(),
					  pObject->getTypeName().c_str());
			}
			else
			{
				Print("ERROR: not registered name=\"%s\" type=\"%s\"",
					  pObject->getName().c_str(),
					  pObject->getTypeName().c_str());
			}
		} break;
	}
}
