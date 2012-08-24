#ifndef D3D11Factory_h__
#define D3D11Factory_h__

#include "cgl.h"

namespace cgl {

//////////////////////////////////////////////////////////////////////////
// typedefs
class CDXGIFactory;
class CDXGIOutput;
class CDXGIAdapter;
typedef CGL_API std::tr1::shared_ptr<CDXGIFactory>	PDXGIFactory;
typedef CGL_API std::tr1::shared_ptr<CDXGIOutput>	PDXGIOutput;
typedef CGL_API std::tr1::shared_ptr<CDXGIAdapter>	PDXGIAdapter;

//////////////////////////////////////////////////////////////////////////
// dxgi factory
class CGL_API CDXGIFactory : public CGLBase<IDXGIFactory>
{
protected:
	CDXGIFactory();

	virtual HRESULT onRestore();
	virtual void onReset();
	void getDependencies(std::vector<PCGLObject>* pDependencies ) { }

public:
	static PDXGIFactory Create();
	UINT GetAdapterCount();
};

//////////////////////////////////////////////////////////////////////////
// dxgi output
class CGL_API CDXGIOutput: public CGLBase<IDXGIOutput>
{
private:
	PDXGIAdapter m_pAdapter;
	UINT m_index;

protected:
	HRESULT onRestore();
	void onReset();
	void getDependencies(std::vector<PCGLObject>* pDependencies);

	CDXGIOutput(PDXGIAdapter pAdapter, UINT index);

public:
	static PDXGIOutput Create(PDXGIAdapter pAdapter, UINT index);
	std::vector<DXGI_MODE_DESC> GetFormats();
};

//////////////////////////////////////////////////////////////////////////
// dxgi adapter
class CGL_API CDXGIAdapter : public CGLBase<IDXGIAdapter>
{
private:
	PDXGIFactory m_pFactory;
	UINT m_index;

protected:
	HRESULT onRestore();
	void onReset();
	void getDependencies(std::vector<PCGLObject>* pDependencies);
	CDXGIAdapter(PDXGIFactory pFactory, UINT index);

public:
	static PDXGIAdapter Create(PDXGIFactory pFactory, UINT index);
	UINT GetOutputCount();
};

//////////////////////////////////////////////////////////////////////////
// get size in bytes from dxgi format
UINT CGL_API GetFormatSize(DXGI_FORMAT format);

}

#endif // D3D11Factory_h__
