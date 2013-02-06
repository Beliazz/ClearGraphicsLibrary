#include "cgl.h"

using namespace cgl;
using namespace cgl::core;

CDXGISwapChain::CDXGISwapChain(cgl::util::PCGLWindow pWindow, PDXGIOutput pOutput, CGLSwapChainDesc desc) 
	: CGLBase("CDXGISwapChain"), m_pWindow(pWindow), m_pOutput(pOutput), m_desc(desc)
{
}


CDXGISwapChain::~CDXGISwapChain(void)
{
}

cgl::core::PDXGISwapChain cgl::core::CDXGISwapChain::Create(cgl::util::PCGLWindow pWindow, PDXGIOutput pOutput, CGLSwapChainDesc desc)
{
	return create(new CDXGISwapChain(pWindow, pOutput, desc));
}

HRESULT cgl::core::CDXGISwapChain::onRestore()
{
	DXGI_SWAP_CHAIN_DESC desc;
	desc.BufferCount = m_desc.bufferCount;
	desc.BufferDesc = m_pOutput->GetFormats().at(m_desc.bufferDescIndex);
	desc.BufferUsage = m_desc.bufferUsage;
	desc.Flags = m_desc.flags;
	desc.OutputWindow = m_pWindow->get();
	desc.SampleDesc.Count = m_desc.samplingCount;
	desc.SampleDesc.Quality = m_desc.samplingQuality;
	desc.SwapEffect = m_desc.swapEffect;
	desc.Windowed = m_desc.windowed;

	return m_pOutput->GetFactory()->get()->CreateSwapChain(CGLAccess::D3DDevice(), &desc, ptr());
}

void cgl::core::CDXGISwapChain::onReset()
{	
	// switch to windowed mode before releasing
	// see msdn for further information
	get()->SetFullscreenState(FALSE, NULL);
	comReset((IUnknown**)ptr());
}

void cgl::core::CDXGISwapChain::getDependencies( std::vector<cgl::core::PCGLObject>* pDependencies )
{
	pDependencies->push_back(m_pOutput);
	pDependencies->push_back(m_pWindow);
}
