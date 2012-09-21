#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// cgl manager connector
cgl::CGLManagerConnector::CGLManagerConnector()
{
	m_pMgr = cgl::CGLManager::GetInstance();
}

//////////////////////////////////////////////////////////////////////////
// CGLObject
UINT cgl::CGLObject::currLUID = 0;

cgl::CGLObject::CGLObject(std::string className) 
	: m_name("unnamed"), m_processing(false), m_restored(false), m_luid(CGLObject::currLUID++), m_typeName(className), m_registered(false)
{
	if (mgr())
	{
		mgr()->Notify(CGL_NOTIFICATION_INSTANTIATION, this);
	}
	else
	{
		CGLLogger::Print("ERROR: no CGLManager instance!");
	}
}
cgl::CGLObject::~CGLObject()
{
	if (mgr())
	{
		mgr()->Notify(CGL_NOTIFICATION_DESCTRUCTION, this);
	}
	else
	{
		CGLLogger::Print("WARNING: object destroyed after the CGL Manager had been deleted");
	}
}

bool cgl::CGLObject::restoreDbg(std::string file, std::string function, long line)
{
	return mgr()->RestoreDbg(this, file, function, line);
}
bool cgl::CGLObject::restore()
{
	return mgr()->Restore(this);
}

void cgl::CGLObject::reset()
{
	mgr()->Reset(this);
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
				// CGLLogger::LogObjectState(CGL_NOTIFICATION_COM_INTERFACE_STILL_ALIVE, this, S_OK, &refCount);
			}

			(*ppComPtr) = NULL;
		}
	}
}
cgl::PD3D11Device cgl::CGLObject::getDevice()
{
	return mgr()->GetDevice();
}

void cgl::CGLObject::setName( std::string name )
{
	m_name = name;
}
std::string cgl::CGLObject::getName()
{
	return m_name;
}

std::string cgl::CGLObject::toString(std::string indent)
{
	char buffer[8192];
	ZeroMemory(buffer, sizeof(char) * 8192);

	sprintf(buffer,
			"%s CGLObject\n"
			"%s {\n"
			"%s    luid: %i\n"
			"%s    name: \"%s\"\n"
			"%s    type: %s\n"
			"%s	   \n"
			"%s	last restoration call\n"
			"%s    {\n"
			"%s       file: \"%s\"\n"
			"%s       function: %s\n"
			"%s       line: %i\n"
			"%s    }\n"
			"%s"
			"%s }\n", 
			indent.c_str(), 
			indent.c_str(), 
			indent.c_str(), m_luid,
			indent.c_str(), m_name.c_str(), 
			indent.c_str(), m_typeName.c_str(),
			indent.c_str(), 
			indent.c_str(), 
			indent.c_str(), 
			indent.c_str(), m_currRestoreFile.c_str(), 
			indent.c_str(), m_currRestoreFunc.c_str(),
			indent.c_str(), m_currRestoreLine,
			indent.c_str(), 
			toStringSpecific(indent + indent).c_str(),
			indent.c_str());

	return std::string(buffer);
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
