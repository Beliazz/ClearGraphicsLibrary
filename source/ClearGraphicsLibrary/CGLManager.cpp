#include "cgl.h"

cgl::core::CGLManager::CGLManager()
{
}

bool cgl::core::CGLManager::Register( PCGLObject pObject )
{
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
	GetObjects().push_back(pObject);

	// mark as registered
	pObject->setRegistered(true);

	return true;
}
bool cgl::core::CGLManager::RegisterDevice( PCGLObject pObject )
{
	// unregister old device
	if (GetDevice())
		GetDevice()->setRegistered(false);

	// assign
	SetDevice(pObject);

	// register new device
	GetDevice()->setRegistered(true);

	return true; 
}
void cgl::core::CGLManager::Unregister( PCGLObject pObject )
{
	Reset(pObject.get());
	pObject->setRegistered(false);
}

bool cgl::core::CGLManager::Restore()
{
	// restore device
	Restore(GetDevice().get());
		
	// restore registered objects
	PCGLObjectList::iterator it;
	for (it = GetObjects().begin(); it != GetObjects().end(); it++)
	{
		if ( !Restore((*it).get()) )
		{
			return false;
		}
	}

	return true;
}
bool cgl::core::CGLManager::Restore( CGLObject* pObject )
{
	if (!pObject->isRestored()) 
	{
		// get dependencies
		std::vector<PCGLObject> dependencies;
		pObject->getDependencies(&dependencies);

		// restore dependencies 
		std::vector<PCGLObject>::iterator it;
		for (it = dependencies.begin(); it != dependencies.end(); it++)
		{
			if (!(*it) || !Restore((*it).get()))
			{
				return false;
			}
		}

		// restore object
		HRESULT result = pObject->onRestore();
		if (FAILED(result))
		{
			pObject->setProcessing(false);
			return false;
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
	}

	return true;
}

bool cgl::core::CGLManager::Reset()
{
	// reset device
	GetDevice()->onReset();
	GetDevice()->setRestored(false);

	// reset registered objects
	PCGLObjectList::reverse_iterator it;
	for (it = GetObjects().rbegin();
		 it != GetObjects().rend();
		 it++)
	{
		if(!Reset((*it).get()))
			return false;
	}

	return true;
}
bool cgl::core::CGLManager::Reset(CGLObject* pObject )
{
	if (pObject->isRestored()) 
	{
		// reset object
		pObject->onReset();

		// reset finished
		pObject->setRestored(false);
	}

	return true;
}

cgl::core::PCGLManager cgl::core::CGLManager::Create()
{
	return CGLManagerBase::create(new CGLManager());
}

cgl::core::CGLManager::~CGLManager()
{
	Tidy();
}







