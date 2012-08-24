#include "cgl.h"

cgl::CGLManager* cgl::CGLManager::m_pInstance = NULL;

//////////////////////////////////////////////////////////////////////////
// CGLManager
cgl::CGLManager::CGLManager()
{
	// reset callback
	m_notificationCallback = NULL;

	if (!m_pInstance)
		CGLLogger::Print("INFO:  CGLManager initialized");
}
cgl::CGLManager::~CGLManager()
{
	// release all objects
	Shutdown();

	m_pInstance = NULL;

	CGLLogger::Print("INFO:  CGLManager shut down");
}

void cgl::CGLManager::Shutdown()
{
	// destroy objects that are garbage
	Tidy();

	// destroy the rest
	CleanUp();
}
void cgl::CGLManager::CleanUp()
{	
	//////////////////////////////////////////////////////////////////////////
	// CORE OBJECTS
	// 
	// objects that were no destroyed by Tidy()
	// are still in use and will be alive after 
	// clearing the list
	//
	// notify application
	// 
	PCGLObjectList::reverse_iterator it;
	for (it = m_coreObjects.rbegin(); it != m_coreObjects.rend(); it++)
	{
		Notify(CGL_NOTIFICATION_STILL_ALIVE, (*it).get(), E_FAIL);
		Unregister(*it);
	}
	//
	// clear list
	m_coreObjects.clear();

	//
	// check device
	if (m_pDevice.use_count() > 1)
	{
		Notify(CGL_NOTIFICATION_STILL_ALIVE, m_pDevice.get(), E_FAIL);
		Unregister(m_pDevice);
	}
	//
	// reset device
	m_pDevice.reset();
}
void cgl::CGLManager::Tidy()
{
	
	//////////////////////////////////////////////////////////////////////////
	// CORE OBJECTS
	// 
	// some objects depend on other
	// so when object 2 depends on object 1
	// object 1 will not be deleted in the first iteration.
	// after object 2 has been deleted object 1 maybe
	// must be deleted too
	// 
	// reversing list
	m_coreObjects.reverse();
	//
	UINT count = 0;
	while(count != m_coreObjects.size())
	{
		count = m_coreObjects.size();
		m_coreObjects.remove_if(IsGarbage);
	}
}

bool cgl::CGLManager::IsGarbage( const PCGLObject& ptr )
{
	// a object is garbage when there are no 
	// external references to this object what means
	// use_count() == 1. But when testing the use count gets
	// raised because of the stack copy. To handle this 
	// the use_count() must be 2  if it is garbage
	if(ptr.use_count() == 1)
	{
		CGLManager::GetInstance()->Unregister(ptr);
		return true;
	}

	return false;
}

bool cgl::CGLManager::Register( PCGLObject pObject )
{
	// validate ptr
	if( !pObject )
	{ 
		Notify(CGL_NOTIFICATION_INVALID_PTR, NULL);
		return false; 
	}

	if (!pObject->registered())
	{
		// if already registering we have cyclic dependency
		if (pObject->processing())
		{
			pObject->setProcessing(false);
			Notify(CGL_NOTIFICATION_CYCLIC_DEPENDENCY, pObject.get());
			return false;
		}
		else
		{
			// indicate that this object is about to be registered
			pObject->setProcessing(true);
		}

		// get dependencies
		std::vector<PCGLObject> dependencies;
		pObject->getDependencies(&dependencies);

		// register dependencies 
		std::vector<PCGLObject>::iterator it;
		for (it = dependencies.begin(); it != dependencies.end(); it++)
		{
			if(!(*it) || !Register(*it))
				return false;
		}

		// add to list
		m_coreObjects.push_back(pObject);

		// mark as registered
		pObject->setRegistered(true);
		pObject->setProcessing(false);

		// notify
		Notify(CGL_NOTIFICATION_REGISTRATION, pObject.get());
	}

	return true;
}
bool cgl::CGLManager::RegisterDevice( PD3D11Device pObject )
{
	// validate ptr
	if( !pObject )
	{ 
		Notify(CGL_NOTIFICATION_INVALID_PTR, NULL);
		return false; 
	}

	if (!pObject->registered())
	{
		// unregister old device
		if (m_pDevice)
			m_pDevice->setRegistered(false);

		// assign
		m_pDevice = pObject;

		// register new device
		m_pDevice->setRegistered(true);

		// reset/restore all because
		// all objects save ptr to 
		// the current device
		Reset();
		if (!Restore())
		{
			return false;
		}

		// notify
		Notify(CGL_NOTIFICATION_REGISTRATION, pObject.get());
	}

	return true; 
}
void cgl::CGLManager::Unregister( PCGLObject pObject )
{
	Reset(pObject.get());
	pObject->setRegistered(false);
}

bool cgl::CGLManager::Restore()
{
	// validate device
	if ( !m_pDevice )
	{
		Notify(CGL_NOTIFICATION_NO_DEVICE, NULL);

		return false; 
	}

	// restore device
	if(!m_pDevice->isRestored())
	{
		HRESULT result = m_pDevice->onRestore();
		Notify(CGL_NOTIFICATION_RESTORATION, m_pDevice.get(), result);

		if (FAILED(result))
		{ 
			return false;
		}	

		// device successfully restored
		m_pDevice->setRestored(true);
	}

	// restore registered objects
	PCGLObjectList::iterator it;
	for (it = m_coreObjects.begin(); it != m_coreObjects.end(); it++)
	{
		if ( !Restore((*it).get()) )
		{
			return false;
		}
	}

	return true;
}
bool cgl::CGLManager::Restore( CGLObject* pObject )
{
	if (!pObject->isRestored()) 
	{
		// if already restoring we have cyclic dependency
		if (pObject->processing())
		{
			pObject->setProcessing(false);
			Notify(CGL_NOTIFICATION_CYCLIC_DEPENDENCY, pObject, S_FALSE);
			return false;
		}
		else
		{
			// indicate that this object is about to be restored
			pObject->setProcessing(true);
		}

		// get dependencies
		std::vector<PCGLObject> dependencies;
		pObject->getDependencies(&dependencies);

		// restore dependencies 
		std::vector<PCGLObject>::iterator it;
		for (it = dependencies.begin(); it != dependencies.end(); it++)
		{
			if (!(*it) || !Restore((*it).get()))
			{
				pObject->setProcessing(false);
				return false;
			}
		}

		// restore object
		HRESULT result = pObject->onRestore();
		Notify(CGL_NOTIFICATION_RESTORATION, pObject, result);

		if (FAILED(result))
		{
			pObject->setProcessing(false);

			return false;
		}

		// restoring finished
		pObject->setRestored(true);
		pObject->setProcessing(false);
	}

	return true;
}

bool cgl::CGLManager::Reset()
{
	// reset device
	m_pDevice->onReset();
	m_pDevice->setRestored(false);

	Notify(CGL_NOTIFICATION_RESET, m_pDevice.get());

	// reset registered objects
	PCGLObjectList::reverse_iterator it;
	for (it = m_coreObjects.rbegin();
		 it != m_coreObjects.rend();
		 it++)
	{
		if(!Reset((*it).get()))
			return false;
	}

	return true;
}
bool cgl::CGLManager::Reset(CGLObject* pObject )
{
	if (pObject->isRestored()) 
	{
		// reset object
		pObject->onReset();
		Notify(CGL_NOTIFICATION_RESET, pObject);

		// reset finished
		pObject->setRestored(false);
		pObject->setProcessing(false);
	}

	return true;
}

void cgl::CGLManager::Notify( UINT notificationType, CGLObject* pObject /*= NULL*/, HRESULT result /*= S_OK */, void* pData )
{
	bool debugOutput = true;
	if (m_notificationCallback)
	{
		debugOutput = m_notificationCallback(notificationType, pObject, result, DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
	}

	if (debugOutput)
	{
		CGLLogger::LogObjectState(notificationType, pObject, result, pData);
	}
}
bool cgl::CGLManager::Depends( PCGLObject objectToCheck, PCGLObject possibleDependency )
{
	if (objectToCheck == possibleDependency)
		return true;

	std::vector<PCGLObject> dependencies;
	objectToCheck->getDependencies(&dependencies);
	for (UINT i = 0; i < dependencies.size(); i++)
	{
		return Depends(dependencies[i], possibleDependency);
	}

	return false;
}

cgl::PCGLManager cgl::CGLManager::Create()
{
	std::tr1::shared_ptr<CGLManager> pInstance = std::tr1::shared_ptr<CGLManager>(new CGLManager());
	m_pInstance = pInstance.get();
	return pInstance;
}
cgl::CGLManager* cgl::CGLManager::GetInstance()
{
	return m_pInstance;
}

// cgl::PCGLObject cgl::CGLManager::GetObject( UINT luid )
// {
// 	for (PCGLObjectList::iterator it = m_coreObjects.begin();
// 		 it != m_coreObjects.end(); it++)
// 	{
// 		if ((*it)->getLuid() == luid)
// 			return (*it);
// 	}
// 	
// 	return nullptr;
// }
// 
// cgl::PCGLObject cgl::CGLManager::GetObjectBinary( __in UINT luid, __in PCGLObjectList::iterator it, __in UINT range )
// {
// 	PCGLObjectList::iterator currIt = it + range / 2;
// 	UINT currluid = (*currIt)->getLuid();
// 	if ( currluid == luid )
// 	{
// 		return (*currIt);
// 	}
// 	else if ( currluid <= luid )
// 	{
// 		return GetObjectBinary(luid, it, range / 2 );
// 	}
// 	else
// 	{
// 		return GetObjectBinary(luid, it, range / 2 )
// 	}
// }






