#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// d3d11 device
cgl::D3D11Device::D3D11Device( std::string className ) : CGLBase(className)
{
	m_bindRegistry.resize(CGL_BIND_UNUSED);
}
void cgl::D3D11Device::onReset()
{
	if (!get())
		return;

	int refs;

	// clear states
	if (get()->pContext)
	{
		(*ptr())->pContext->ClearState();
		(*ptr())->pContext->Flush();
	}
	
	// switch to windowed mode before releasing
	// see msdn for further information
	if ( get()->pSwapChain )
	{
		get()->pSwapChain->SetFullscreenState(FALSE, NULL);
		refs = (*ptr())->pSwapChain->Release();
		(*ptr())->pSwapChain = NULL;
	}

	if((*ptr())->pContext)
	{
		refs = (*ptr())->pContext->Release();
		(*ptr())->pContext = NULL;
	}
	
	if((*ptr())->pDevice)
	{
		refs = (*ptr())->pDevice->Release();
		(*ptr())->pDevice = NULL;
	}

	delete get();
	set(NULL);
}
bool cgl::D3D11Device::Register( PD3D11Device pDevice )
{
	if (pDevice)
		if(CGLManager::GetInstance()->RegisterDevice(pDevice))
			return true;

	return false;
}
void cgl::D3D11Device::Bind( CGLBindable* pBindable )
{
	if (!pBindable->valid() || m_bindRegistry[static_cast<int>(pBindable->getBind())] != pBindable->getBindId())
	{
		pBindable->bind();
		pBindable->validate();
		m_bindRegistry[static_cast<int>(pBindable->getBind())] = pBindable->getBindId();
	}
}

//////////////////////////////////////////////////////////////////////////
// d3d11 device class
cgl::CD3D11Device::CD3D11Device( CGL_D3D11_DEVICE_DESC desc ) : D3D11Device("CD3D11Device"), m_desc(desc) { }
HRESULT cgl::CD3D11Device::onRestore()
{
	set(new CGL_D3D11_DEVICE());

	D3D_FEATURE_LEVEL supportedLevel;
	return D3D11CreateDeviceAndSwapChain(m_desc.pAdapter, 
		m_desc.driverType, 
		NULL, 
		m_desc.flags, 
		&m_desc.featureLevel,
		1, 
		D3D11_SDK_VERSION,
		&(m_desc.swapDesc),
		&((*ptr())->pSwapChain),
		&((*ptr())->pDevice), 
		&supportedLevel, 
		&((*ptr())->pContext));
}
std::tr1::shared_ptr<cgl::CD3D11Device> cgl::CD3D11Device::Create( CGL_D3D11_DEVICE_DESC desc )
{
	std::tr1::shared_ptr<CD3D11Device> pInstance(new CD3D11Device(desc));

	if(Register(pInstance))
		return pInstance;

	return nullptr;
}



