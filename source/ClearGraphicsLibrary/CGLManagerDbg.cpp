#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// cgl debug manager
cgl::debug::CGLManagerDbg::CGLManagerDbg()
{
}
cgl::debug::CGLManagerDbg::~CGLManagerDbg()
{
	CGLTraceFunc();

	//////////////////////////////////////////////////////////////////////////
	// CORE OBJECTS
	// 
	// objects that are not destroyed by Tidy()
	// are still in use and will be alive after 
	// clearing the list
	//
	// notify application
	// 
	// check device
	Tidy();
	
	if (GetDevice().use_count() > 1)
		Notify(CGL_NOTIFICATION_STILL_ALIVE, GetDevice().get());	

	ID3D11Debug* pDebug = NULL;
	reinterpret_cast<cgl::core::CD3D11Device*>(GetDevice().get())->get()->QueryInterface(__uuidof(ID3D11Debug), (void**)&pDebug);
	if(pDebug)
	{
		Unregister(GetDevice());
		SetDevice(nullptr);

		Tidy();
		for (auto it = GetObjects().rbegin(); it != GetObjects().rend(); it++)
		{
			Notify(CGL_NOTIFICATION_STILL_ALIVE, (*it).get());
			Unregister(*it);
		}

		pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		pDebug->Release();
	}
	else
	{
		Unregister(GetDevice());
		SetDevice(nullptr);

		Tidy();
		for (auto it = GetObjects().rbegin(); it != GetObjects().rend(); it++)
		{
			Notify(CGL_NOTIFICATION_STILL_ALIVE, (*it).get());
			Unregister(*it);
		}
	}
}

bool cgl::debug::CGLManagerDbg::Register( cgl::core::PCGLObject pObject )
{
	CGLTraceFunc();
	CGLInfo((std::string("[") + pObject->getTypeName() + std::string("]")).c_str());

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
		std::vector<cgl::core::PCGLObject> dependencies;
		pObject->getDependencies(&dependencies);

		// register dependencies 
		std::vector<cgl::core::PCGLObject>::iterator it;
		for (it = dependencies.begin(); it != dependencies.end(); it++)
		{
			if(!(*it) || !Register(*it))
				return false;
		}

		// add to list
		GetObjects().push_back(pObject);

		// mark as registered
		pObject->setRegistered(true);
		pObject->setProcessing(false);

		// notify
		Notify(CGL_NOTIFICATION_REGISTRATION, pObject.get());
	}

	return true;
}
bool cgl::debug::CGLManagerDbg::RegisterDevice( cgl::core::PCGLObject pObject )
{
	CGLTraceFunc();
	CGLInfo((std::string("[") + pObject->getTypeName() + std::string(" ") + pObject->getName() + std::string("]")).c_str());

	// validate ptr
	if( !pObject )
	{ 
		Notify(CGL_NOTIFICATION_INVALID_PTR, NULL);
		return false; 
	}

	if (!pObject->registered())
	{
		// unregister old device
		if (GetDevice())
			Unregister(GetDevice());

		// assign
		SetDevice(pObject);

		// register new device
		GetDevice()->setRegistered(true);

		// reset/restore all because
		// all objects save ptr to 
		// the current device
		Reset();
		if (!Restore())
		{
			// notify
			Notify(CGL_NOTIFICATION_REGISTRATION_FAILED, pObject.get());

			return false;
		}

		// notify
		Notify(CGL_NOTIFICATION_REGISTRATION, pObject.get());
	}

	return true; 
}
void cgl::debug::CGLManagerDbg::Unregister( cgl::core::PCGLObject pObject )
{
	CGLTraceFunc();
	CGLInfo((std::string("[") + pObject->getTypeName() + std::string(" ") + pObject->getName() + std::string("]")).c_str());

	Reset(pObject.get());
	pObject->setRegistered(false);
	Notify(CGL_NOTIFICATION_UNREGISTRATION, pObject.get());
}

bool cgl::debug::CGLManagerDbg::Restore()
{
	CGLTraceFunc();

	// validate device
	if (!GetDevice())
	{
		Notify(CGL_NOTIFICATION_NO_DEVICE, NULL);
		return false; 
	}

	// restore device
	if(!Restore(GetDevice().get()))
		return false;

	// restore registered objects
	cgl::core::PCGLObjectList::iterator it;
	for (it = GetObjects().begin(); it != GetObjects().end(); it++)
	{
		if ( !Restore((*it).get()) )
		{
			return false;
		}
	}

	return true;
}
bool cgl::debug::CGLManagerDbg::Restore( cgl::core::CGLObject* pObject )
{
	CGLTraceFunc();
	CGLInfo((std::string("[") + pObject->getTypeName() + std::string(" ") + pObject->getName() + std::string("]")).c_str());

	if (!pObject->isRestored()) 
	{
		// if already restoring we have cyclic dependency
		if (pObject->processing())
		{
			pObject->setProcessing(false);
			Notify(CGL_NOTIFICATION_CYCLIC_DEPENDENCY, pObject);
			return false;
		}
		else
		{
			// indicate that this object is about to be restored
			pObject->setProcessing(true);
		}

		// get dependencies
		std::vector<cgl::core::PCGLObject> dependencies;
		pObject->getDependencies(&dependencies);

		// restore dependencies 
		std::vector<cgl::core::PCGLObject>::iterator it;
		for (it = dependencies.begin(); it != dependencies.end(); it++)
		{
			if (!(*it) || !(*it)->restore())
			{
				pObject->setProcessing(false);
				return false;
			}
		}

		// restore object
		HRESULT result = pObject->onRestore();
		pObject->setLastResult(result);
		if (FAILED(result))
		{
			Notify(CGL_NOTIFICATION_RESTORATION_FAILED, pObject);
			pObject->setProcessing(false);
			return false;
		}
		else
		{
			Notify(CGL_NOTIFICATION_RESTORATION, pObject);
		}

		// if this object can be bound to the render pipeline
		// invalidate its state so it is bound again next time
		cgl::core::CGLBindable* pBindable = reinterpret_cast<cgl::core::CGLBindable*>(pObject);
		if(pBindable)
		{
			pBindable->invalidate();
		}

		// restoring finished
		pObject->setRestored(true);
		pObject->setProcessing(false);
	}

	return true;
}

bool cgl::debug::CGLManagerDbg::Reset()
{
	CGLTraceFunc();

	// reset device
	Reset(GetDevice().get());
	Notify(CGL_NOTIFICATION_RESET, GetDevice().get());

	// reset registered objects
	cgl::core::PCGLObjectList::reverse_iterator it;
	for (it = GetObjects().rbegin();
		it != GetObjects().rend();
		it++)
	{
		if(!Reset((*it).get()))
			return false;
	}

	return true;
}
bool cgl::debug::CGLManagerDbg::Reset( cgl::core::CGLObject* pObject )
{
	CGLTraceFunc();
	CGLInfo((std::string("[") + pObject->getTypeName() + std::string(" ") + pObject->getName() + std::string("]")).c_str());

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

cgl::core::PCGLManager cgl::debug::CGLManagerDbg::Create()
{
	return CGLManagerBase::create(new CGLManagerDbg());
}
void cgl::debug::CGLManagerDbg::Notify( UINT notificationType, cgl::core::CGLObject* pObject /*= NULL*/, void* pData /*= NULL */ )
{
	const std::string fi = __FILE__;
	const UINT li = __LINE__;
	const UINT errLt = -1;
	const UINT infLt = 0;
	const UINT warnLt = 0;

	switch(notificationType)
	{
	case CGL_NOTIFICATION_STILL_ALIVE:			CGLLogEntry(fi, li, CGL_LOG_CATEGORY_WARNING, "object still alive", pObject); break;
	
	case CGL_NOTIFICATION_NO_DEVICE:			CGLLogEntry(fi, li, CGL_LOG_CATEGORY_ERROR, "no device registered", pObject); break;
	case CGL_NOTIFICATION_CYCLIC_DEPENDENCY:	CGLLogEntry(fi, li, CGL_LOG_CATEGORY_ERROR, "cyclic dependency", pObject); break;
	case CGL_NOTIFICATION_INVALID_PTR:			CGLLogEntry(fi, li, CGL_LOG_CATEGORY_ERROR, "passed ptr is invalid", pObject); break;
	case CGL_NOTIFICATION_RESTORATION_FAILED:	CGLLogEntry(fi, li, CGL_LOG_CATEGORY_ERROR, "object restoration failed", pObject); break;
	case CGL_NOTIFICATION_CREATION_FAILED:		CGLLogEntry(fi, li, CGL_LOG_CATEGORY_ERROR, "object creation failed (no cgl manager instance)", pObject); break;
	case CGL_NOTIFICATION_REGISTRATION_FAILED:	CGLLogEntry(fi, li, CGL_LOG_CATEGORY_ERROR, "object registration failed", pObject); break;
	case CGL_NOTIFICATION_COM_INTERFACE_STILL_ALIVE: CGLLogEntry(fi, li, CGL_LOG_CATEGORY_ERROR, "'COM interface still alive", pObject); break;

	case CGL_NOTIFICATION_REGISTRATION:			CGLLogEntry(fi, li, CGL_LOG_CATEGORY_INFO, "object registered", pObject); break;
	case CGL_NOTIFICATION_UNREGISTRATION:		CGLLogEntry(fi, li, CGL_LOG_CATEGORY_INFO, "object unregistered", pObject); break;
	case CGL_NOTIFICATION_RESET:				CGLLogEntry(fi, li, CGL_LOG_CATEGORY_INFO, "object reset", pObject); break;
	case CGL_NOTIFICATION_MGR_INITIALIZED:		CGLLogEntry(fi, li, CGL_LOG_CATEGORY_INFO, "manager initialized", pObject); break;
	case CGL_NOTIFICATION_MGR_SHUTDOWN:			CGLLogEntry(fi, li, CGL_LOG_CATEGORY_INFO, "manager shut down", pObject); break;
	case CGL_NOTIFICATION_CREATION:				CGLLogEntry(fi, li, CGL_LOG_CATEGORY_INFO, "object created", pObject); break;
	case CGL_NOTIFICATION_RESTORATION:			CGLLogEntry(fi, li, CGL_LOG_CATEGORY_INFO, "object restored", pObject); break;

	default: CGLLogEntry(fi, li, CGL_LOG_CATEGORY_ERROR, "unknown notification", pObject); break;
	}
}

