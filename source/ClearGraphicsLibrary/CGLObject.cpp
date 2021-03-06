#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// cgl access
cgl::core::ComPtr<ID3D11DeviceContext> cgl::core::CGLAccess::D3DContext()
{
	ID3D11DeviceContext* pContext = NULL;
	reinterpret_cast<CD3D11Device*>(CGLManagerBase::GetInstance()->GetDevice().get())->get()->GetImmediateContext(&pContext);
	return pContext;
}

ID3D11Device* cgl::core::CGLAccess::D3DDevice()
{
	return reinterpret_cast<CD3D11Device*>(CGLManagerBase::GetInstance()->GetDevice().get())->get();
}
cgl::core::CD3D11Device* cgl::core::CGLAccess::CGLDevice()
{
	return reinterpret_cast<CD3D11Device*>(CGLManagerBase::GetInstance()->GetDevice().get());
}
cgl::core::CGLManagerBase* cgl::core::CGLAccess::CGLMgr()
{
	return CGLManagerBase::GetInstance();
}


//////////////////////////////////////////////////////////////////////////
// CGLObject
UINT cgl::core::CGLObject::currLUID = 0;

cgl::core::CGLObject::CGLObject(std::string className) 
	: m_processing(false), m_restored(false), m_luid(CGLObject::currLUID++), m_typeName(className), m_registered(false), m_name("unnamed")
{}
cgl::core::CGLObject::~CGLObject()
{
}

bool cgl::core::CGLObject::restore()
{
	return CGLAccess::CGLMgr()->Restore(this);
}
void cgl::core::CGLObject::reset()
{
	CGLAccess::CGLMgr()->Reset(this);
}

void cgl::core::CGLObject::comReset(IUnknown** ppComPtr )
{
	if (ppComPtr)
	{
		if (*ppComPtr)
		{
			int refCount = (*ppComPtr)->Release();
			if (refCount > 0)
			{
				std::stringstream ss;
				ss << "interface still alive [ref count = " << refCount << "]";
				cgl::debug::CGLLogEntry(__FILE__, __LINE__, cgl::debug::CGL_LOG_CATEGORY_WARNING, ss.str().c_str(), this);
			}

			(*ppComPtr) = NULL;
		}
	}
}



