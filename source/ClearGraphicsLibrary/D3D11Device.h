#ifndef D3D11Device_h__
#define D3D11Device_h__

#include "cgl.h"

namespace cgl
{

//////////////////////////////////////////////////////////////////////////
// typedef
class D3D11Device;
typedef CGL_API std::tr1::shared_ptr<D3D11Device>  PD3D11Device;

//////////////////////////////////////////////////////////////////////////
// d3d device base class
struct CGL_API CGL_D3D11_DEVICE
{
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	IDXGISwapChain* pSwapChain;

	CGL_D3D11_DEVICE() : pDevice(NULL), pContext(NULL), pSwapChain(NULL) { }

};
class CGL_API D3D11Device : public CGLBase<CGL_D3D11_DEVICE>
{
friend class CGLManager;

protected:	
	D3D11Device(std::string className);
			
	void onReset();
	void getDependencies(std::vector<PCGLObject>* pDependencies ) { }
	static bool Register(PD3D11Device pDevice);

	std::vector<UINT> m_bindRegistry;

public:
	virtual ~D3D11Device() { onReset(); }

	ID3D11DeviceContext* GetContext()	{ return get()->pContext;  }
	ID3D11Device*		 GetDevice()	{ return get()->pDevice;   }
	IDXGISwapChain*		 GetSwapChain()	{ return get()->pSwapChain;}

	void Bind(CGLBindable* pBindable);
	UINT GetCurrentlyBoundObject(CGL_BIND type) { return m_bindRegistry[static_cast<int>(type)]; }
};

//////////////////////////////////////////////////////////////////////////
// d3d11 device class implementation
// 
struct CGL_API CGL_D3D11_DEVICE_DESC
{
	IDXGIAdapter* pAdapter;
	D3D_DRIVER_TYPE driverType;
	UINT flags;
	D3D_FEATURE_LEVEL featureLevel;
	DXGI_SWAP_CHAIN_DESC swapDesc;
};
class CGL_API CD3D11Device : public D3D11Device
{
protected:
	CGL_D3D11_DEVICE_DESC m_desc;
	CD3D11Device(CGL_D3D11_DEVICE_DESC desc);

public:
	HRESULT onRestore();
	static std::tr1::shared_ptr<CD3D11Device> Create (CGL_D3D11_DEVICE_DESC desc);
};

}

#endif // D3D11Device_h__
