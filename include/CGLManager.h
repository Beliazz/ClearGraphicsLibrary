#ifndef D3D11Manager_h__
#define D3D11Manager_h__

#include "cgl.h"

namespace cgl {

typedef bool (CALLBACK*  CGLNotificationCallback)(UINT, CGLObject*, HRESULT, std::string, std::string);
typedef std::tr1::shared_ptr<CGLManager> PCGLManager;

class CGL_API CGLManager
{
friend class CGLManagerConnector;
friend class CGLObject;
friend class D3D11Device;

private:
	//////////////////////////////////////////////////////////////////////////
	// static ptr
	//
	// for the notification callback 
	// if a new CGLBase object is created
	// it will send a notification to 
	// the application through this ptr
	static CGLManager* m_pInstance;

	//////////////////////////////////////////////////////////////////////////
	// d3d11 device
	//
	// this member contains the core components
	// of every d3d app: the device, the device context
	// and the swap chain
	PD3D11Device m_pDevice;

	//////////////////////////////////////////////////////////////////////////
	// cgl objects
	PCGLObjectList m_coreObjects;
	PCGLObjectList m_nonCoreObjects;

	//////////////////////////////////////////////////////////////////////////
	// track object count
	UINT m_numObjectsRegistered;
	UINT m_numObjectsDead;

	//////////////////////////////////////////////////////////////////////////
	// cleanUp
	//
	// this function will remind
	// the application to delete unused
	// objects an clears the d3d11object list
	void CleanUp();

	//////////////////////////////////////////////////////////////////////////
	// shutdown
	//
	// is called in the destructor
	// it first calls tidy() and then CleanUp()
	void Shutdown();

	//////////////////////////////////////////////////////////////////////////
	// garbage predicate
	static bool IsGarbage(const PCGLObject& ptr);

	//////////////////////////////////////////////////////////////////////////
	// notification callback
	//
	CGLNotificationCallback m_notificationCallback;
	
	//////////////////////////////////////////////////////////////////////////
	// check if a d3d11 object has any dependencies on the other
	bool Depends(PCGLObject objectToCheck, PCGLObject possibleDependency);

	//////////////////////////////////////////////////////////////////////////
	// get object with binary search
	PCGLObject CGLManager::GetObjectBinary( UINT luid, PCGLObjectList::iterator start, UINT range);

	//////////////////////////////////////////////////////////////////////////
	// reset
	bool Reset(CGLObject* pObject);
	//
	//
	// restore
	bool Restore(CGLObject* pObject);

	//////////////////////////////////////////////////////////////////////////
	// register object
	//
	// will only register objects 
	//
	// call CGLManager::Restore after registering 
	// all objects
	//
	// this must be separated because of 
	// possible dependencies 
	bool RegisterDevice(PD3D11Device pObject);
	bool Register(PCGLObject pObject);
	void Unregister(PCGLObject pObject);
	void Release(PCGLObject& pObject);

	CGLManager();
	static CGLManager* GetInstance();

public:	
	static PCGLManager Create();
	
	~CGLManager();

	//////////////////////////////////////////////////////////////////////////
	// get object from luid
	// PCGLObject GetObject(UINT luid);

	//////////////////////////////////////////////////////////////////////////
	// reset
	bool Reset();
	//
	// restore
	bool Restore();

	//////////////////////////////////////////////////////////////////////////
	inline PD3D11Device& GetDevice() { return m_pDevice; }

	//////////////////////////////////////////////////////////////////////////
	// set notification callback
	// 
	// the return value of the callback function indicates
	// if this notification should be forwarded to the debug output window
	void SetNotificationCallback(CGLNotificationCallback callback) { m_notificationCallback = callback; }
	
	//////////////////////////////////////////////////////////////////////////
	// notify application
	//
	// will call the notification callback if set
	void Notify ( UINT notificationType, CGLObject* pObject = NULL, HRESULT result = S_OK, void* pData = NULL );

	//////////////////////////////////////////////////////////////////////////
	// clean manager from unused objects
	void Tidy();
};

typedef CGL_API std::tr1::shared_ptr<CGLManager> PCGLManager;

}

#endif // D3D11Manager_h__