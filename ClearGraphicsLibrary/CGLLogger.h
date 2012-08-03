#ifndef CGLLogger_h__
#define CGLLogger_h__

namespace cgl {

#define CGL_LOG_BUFFER 4096

enum CGL_NOTIFICATION
{
	CGL_NOTIFICATION_INVALID_PTR,
	CGL_NOTIFICATION_NO_DEVICE,
	CGL_NOTIFICATION_INSTANTIATION,
	CGL_NOTIFICATION_CREATION,
	CGL_NOTIFICATION_RESTORATION,
	CGL_NOTIFICATION_CYCLIC_DEPENDENCY,
	CGL_NOTIFICATION_REGISTRATION,
	CGL_NOTIFICATION_RESET,
	CGL_NOTIFICATION_DESCTRUCTION,
	CGL_NOTIFICATION_STILL_ALIVE,
	CGL_NOTIFICATION_COM_INTERFACE_STILL_ALIVE
};

class CGL_API CGLLogger
{
friend class CGLManager;
friend class CGLObject;

private:
	static void LogObjectState(UINT logType, cgl::CGLObject* pObject, HRESULT result = S_OK, void* pData = NULL);

public:
	static bool Print(LPCSTR log, ...);
};

}

#endif // CGLLogger_h__
