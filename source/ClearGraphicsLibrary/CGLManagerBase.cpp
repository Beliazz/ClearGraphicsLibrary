#include "cgl.h"

cgl::core::CGLManagerBase* cgl::core::CGLManagerBase::m_pInstance = NULL;

bool cgl::core::CGLManagerBase::IsGarbage( const std::tr1::shared_ptr<CGLObject>& ptr )
{
	// a object is garbage when there are no 
	// external references to this object what means
	// use_count() == 1. But when testing the use count gets
	// raised because of the stack copy. To handle this 
	// the use_count() must be 2  if it is garbage
	if(ptr.use_count() == 1)
	{
		CGLManagerBase::GetInstance()->Unregister(ptr);
		return true;
	}

	return false;
}

void cgl::core::CGLManagerBase::Tidy()
{
	//////////////////////////////////////////////////////////////////////////
	// CORE OBJECTS
	// 
	// reversing list
	m_objects.reverse();
	//
	UINT count = 0;
	while(count != m_objects.size())
	{
		count = m_objects.size();
		m_objects.remove_if(IsGarbage);
	}
	//
	// reverse back to the normal order
	m_objects.reverse();
}

cgl::core::CGLManagerBase::CGLManagerBase()
{
	
}

std::tr1::shared_ptr<cgl::core::CGLManagerBase> cgl::core::CGLManagerBase::create( CGLManagerBase* pData )
{
	std::tr1::shared_ptr<CGLManagerBase> pInstance = std::tr1::shared_ptr<CGLManagerBase>(pData);
	CGLManagerBase::m_pInstance = pInstance.get();
	return pInstance;
}

cgl::core::CGLManagerBase* cgl::core::CGLManagerBase::GetInstance()
{
	return m_pInstance;
}
