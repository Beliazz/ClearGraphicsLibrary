#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// d3d11 device
void cgl::core::CD3D11Device::onReset()
{
	// clear states
	ID3D11DeviceContext* pContext = NULL;
	get()->GetImmediateContext(&pContext);

	pContext->ClearState();
	pContext->Flush();
	pContext->Release();

	comReset((IUnknown**)ptr());
}
void cgl::core::CD3D11Device::Bind( CGLBindable* pBindable )
{
	if (!pBindable->valid() || m_bindRegistry[static_cast<int>(pBindable->getBind())] != pBindable->getId())
	{
		pBindable->bind();
		pBindable->validate();
		m_bindRegistry[static_cast<int>(pBindable->getBind())] = pBindable->getId();
	}
}

cgl::core::CD3D11Device::CD3D11Device( PDXGIAdapter pAdapter, UINT flags) 
	:  CGLBase("CD3D11Device"), m_pAdapter(pAdapter), m_flags(flags)
{
	m_bindRegistry.resize(CGL_BIND_UNUSED);
}
HRESULT cgl::core::CD3D11Device::onRestore()
{
	return D3D11CreateDevice(m_pAdapter->get(), 
							 D3D_DRIVER_TYPE_UNKNOWN, 
							 NULL, 
							 m_flags, 
							 NULL, 
							 0, 
							 D3D11_SDK_VERSION,
							 ptr(), 
							 &m_featureLevel, 
							 NULL); 

}
cgl::core::PD3D11Device cgl::core::CD3D11Device::Create( PDXGIAdapter pAdapter, UINT flags )
{
	PD3D11Device pInstance(new CD3D11Device(pAdapter, flags));
	if(CGLManager::GetInstance()->RegisterDevice(pInstance))
		return pInstance;

	return nullptr;
}

void cgl::core::CD3D11Device::getDependencies( std::vector<PCGLObject>* pDependencies )
{
	pDependencies->push_back(m_pAdapter);
}



