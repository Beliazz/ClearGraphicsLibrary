#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// dxgi factory
cgl::core::CDXGIFactory::CDXGIFactory() : CGLBase("CD3D11Factory")
{

}
HRESULT cgl::core::CDXGIFactory::onRestore( )
{
	return CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)ptr());
}
void cgl::core::CDXGIFactory::onReset()
{
	comReset((IUnknown**)ptr());
}
cgl::core::PDXGIFactory cgl::core::CDXGIFactory::Create()
{
	return create<CDXGIFactory>(new CDXGIFactory());
}
UINT cgl::core::CDXGIFactory::GetAdapterCount()
{
	IDXGIAdapter* pAdapter;
	UINT count = 0;
	for (UINT i = 0; get()->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND ; i++)
	{
		SAFE_RELEASE(pAdapter);
		count++;
	}

	return count;
}

//////////////////////////////////////////////////////////////////////////
// dxgi output
HRESULT cgl::core::CDXGIOutput::onRestore()
{
	return m_pAdapter->get()->EnumOutputs(m_index, ptr());
}
void cgl::core::CDXGIOutput::onReset()
{
	comReset((IUnknown**)ptr());
}
void cgl::core::CDXGIOutput::getDependencies( std::vector<PCGLObject>* pDependencies )
{
	pDependencies->push_back(m_pAdapter);
}
cgl::core::CDXGIOutput::CDXGIOutput( PDXGIAdapter pAdapter, UINT index )
	: CGLBase("CDXGIOutput"), m_pAdapter(pAdapter), m_index(index)
{

}
cgl::core::PDXGIOutput cgl::core::CDXGIOutput::Create( PDXGIAdapter pAdapter, UINT index )
{
	return create(new CDXGIOutput(pAdapter, index));
}
std::vector<DXGI_MODE_DESC> cgl::core::CDXGIOutput::GetFormats()
{
	std::vector<DXGI_MODE_DESC> descs;

	for (int i = 0; i < DXGI_FORMAT_BC7_UNORM_SRGB; i++)
	{
		UINT numModes = 0;
		get()->GetDisplayModeList((DXGI_FORMAT)i, 0, &numModes, 0);

		if(numModes > 0)
		{
			DXGI_MODE_DESC* pDescs = new DXGI_MODE_DESC[numModes];
			get()->GetDisplayModeList((DXGI_FORMAT)i, 0, &numModes, pDescs);

			for (UINT j = 0; j < numModes; j++)
				descs.push_back(pDescs[j]);

			SAFE_DELETE_ARRAY(pDescs);
		}
	}

	return descs;
}

//////////////////////////////////////////////////////////////////////////
// dxgi adapter
void cgl::core::CDXGIAdapter::onReset()
{
	comReset((IUnknown**)ptr());
}
HRESULT cgl::core::CDXGIAdapter::onRestore()
{
	return m_pFactory->get()->EnumAdapters(m_index, ptr());
}
void cgl::core::CDXGIAdapter::getDependencies( std::vector<PCGLObject>* pDependencies )
{
	pDependencies->push_back(m_pFactory);
}
cgl::core::CDXGIAdapter::CDXGIAdapter( PDXGIFactory pFactory, UINT index )
	: CGLBase("CDXGIAdapter"), m_pFactory(pFactory), m_index(index)
{

}
cgl::core::PDXGIAdapter cgl::core::CDXGIAdapter::Create( PDXGIFactory pFactory, UINT index )
{
	return create(new CDXGIAdapter(pFactory, index));
}
UINT cgl::core::CDXGIAdapter::GetOutputCount()
{
	IDXGIOutput* pOutput;
	UINT count = 0;
	for (UINT i = 0; get()->EnumOutputs(i, &pOutput) != DXGI_ERROR_NOT_FOUND ; i++)
	{
		SAFE_RELEASE(pOutput);
		count++;
	}

	return count;
}

//////////////////////////////////////////////////////////////////////////
// get format size in bytes
UINT CGL_API cgl::core::GetFormatSize( DXGI_FORMAT format )
{
	switch(format)
	{
	case DXGI_FORMAT_R1_UNORM:
		return 1;

	case DXGI_FORMAT_A8_UNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_UNORM:
		return 8;

	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_UNORM:

	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_UNORM:

	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		return 16;

	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_D32_FLOAT:

	case DXGI_FORMAT_G8R8_G8B8_UNORM:
	case DXGI_FORMAT_R8G8_B8G8_UNORM:

	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R11G11B10_FLOAT:

	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_TYPELESS:

	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:

	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:

	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		return 32;

	default:
		return 0;
	}
}