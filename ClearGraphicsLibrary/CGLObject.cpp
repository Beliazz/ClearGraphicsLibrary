#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// CGLBase
UINT cgl::CGLObject::currLUID = 0;

cgl::CGLObject::CGLObject(std::string className) 
	: m_name("unknown"), m_processing(false), m_restored(false), m_luid(CGLObject::currLUID++), m_typeName(className), m_registered(false)
{
	if (CGLManager::GetInstance())
	{
		CGLManager::GetInstance()->Notify(CGL_NOTIFICATION_INSTANTIATION, this);
	}
	else
	{
		CGLLogger::Print("ERROR: no CGLManager instance!");
	}
}
cgl::CGLObject::~CGLObject()
{
	if (CGLManager::GetInstance())
	{
		CGLManager::GetInstance()->Notify(CGL_NOTIFICATION_DESCTRUCTION, this);
	}
	else
	{
		CGLLogger::Print("WARNING: object destroyed after the CGL Manager had been deleted");
	}
}

bool cgl::CGLObject::restore()
{
	return CGLManager::GetInstance()->Restore(this);
}
void cgl::CGLObject::reset()
{
	CGLManager::GetInstance()->Reset(this);
}

void cgl::CGLObject::comReset(IUnknown** ppComPtr )
{
	if (ppComPtr)
	{
		if (*ppComPtr)
		{
			int refCount = (*ppComPtr)->Release();
			if (refCount > 0)
			{
				CGLLogger::LogObjectState(CGL_NOTIFICATION_COM_INTERFACE_STILL_ALIVE, this, S_OK, &refCount);
			}

			(*ppComPtr) = NULL;
		}
	}
}
cgl::PD3D11Device cgl::CGLObject::getDevice()
{
	return CGLManager::GetInstance()->GetDevice();
}

void cgl::CGLObject::setName( std::string name )
{
	m_name = name;
}
std::string cgl::CGLObject::getName()
{
	return m_name;
}

//////////////////////////////////////////////////////////////////////////
// CGLPrivateData
// cgl::CGLPrivateData::CGLPrivateData( CGLObject* pParent, std::string name ) 
// 	: m_pParent(pParent), m_name(name)
// {
// 
// }
// cgl::CGLPrivateData::~CGLPrivateData()
// {
// 	resetData();
// }
// void cgl::CGLPrivateData::setData( void* pData, UINT size )
// {
// 	resetData();
// 	m_data.resize(size);
// 	memcpy(m_data.data(), pData, size);
// }
// void cgl::CGLPrivateData::addData( void* pData, UINT size )
// {
// 	m_data.resize(m_data.size() + size);
// 	memcpy(m_data.data(), pData, size);
// }
// void cgl::CGLPrivateData::resetData()
// {
// 	m_data.clear();
// }
// void cgl::CGLPrivateData::dumpData()
// {
// 
// }
