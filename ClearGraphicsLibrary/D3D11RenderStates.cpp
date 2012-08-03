#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// rasterizer state
cgl::CD3D11RasterizerState::CD3D11RasterizerState(D3D11_RASTERIZER_DESC desc) : CGLBase("CD3D11RasterizerState"), m_desc(desc)
{ }
HRESULT cgl::CD3D11RasterizerState::onRestore()
{
	return getDevice()->GetDevice()->CreateRasterizerState(&m_desc, ptr());
}
void cgl::CD3D11RasterizerState::Bind()
{
	getDevice()->GetContext()->RSSetState(get());
}
std::tr1::shared_ptr<cgl::CD3D11RasterizerState> cgl::CD3D11RasterizerState::Create( D3D11_RASTERIZER_DESC desc )
{
	return create<CD3D11RasterizerState>(new CD3D11RasterizerState(desc));
}
void cgl::CD3D11RasterizerState::onReset()
{
	comReset((IUnknown**)ptr());
}

//////////////////////////////////////////////////////////////////////////
// blend state
cgl::CD3D11BlendState::CD3D11BlendState(D3D11_BLEND_DESC desc, const float stdBlendFactor[4], UINT stdSampleMask) : CGLBase("CD3D11BlendState"), m_desc(desc), m_stdSampleMask(stdSampleMask)
{
	memcpy(&m_stdBlendFactor[0], &stdBlendFactor[0], 4 * sizeof(float) );
}
HRESULT cgl::CD3D11BlendState::onRestore()
{
	return getDevice()->GetDevice()->CreateBlendState(&m_desc, ptr());
}
void cgl::CD3D11BlendState::onReset()
{
	comReset((IUnknown**)ptr());
}
void cgl::CD3D11BlendState::Bind()
{
	Bind(m_stdBlendFactor, m_stdSampleMask);
}
void cgl::CD3D11BlendState::Bind( float blendFactor[4], UINT sampleMask )
{
	getDevice()->GetContext()->OMSetBlendState(get(), blendFactor, sampleMask);
}
std::tr1::shared_ptr<cgl::CD3D11BlendState> cgl::CD3D11BlendState::Create( D3D11_BLEND_DESC desc, const float stdBlendFactor[4], UINT stdSampleMask )
{
	return create<CD3D11BlendState>(new CD3D11BlendState( desc, stdBlendFactor, stdSampleMask ));
}

//////////////////////////////////////////////////////////////////////////
// depth stencil state
cgl::CD3D11DepthStencilState::CD3D11DepthStencilState(D3D11_DEPTH_STENCIL_DESC desc, UINT8 stdStencilRef) : CGLBase("CD3D11DepthStencilState"), m_desc(desc)  { }
HRESULT cgl::CD3D11DepthStencilState::onRestore()
{
	return getDevice()->GetDevice()->CreateDepthStencilState(&m_desc, ptr());
}
void cgl::CD3D11DepthStencilState::onReset()
{
	comReset((IUnknown**)ptr());
}
void cgl::CD3D11DepthStencilState::Bind()
{
	Bind(m_stdStencilRef);
}
void cgl::CD3D11DepthStencilState::Bind( UINT8 stencilRef )
{
	getDevice()->GetContext()->OMSetDepthStencilState(get(), stencilRef);
}
std::tr1::shared_ptr<cgl::CD3D11DepthStencilState> cgl::CD3D11DepthStencilState::Create( D3D11_DEPTH_STENCIL_DESC desc, UINT8 stdStencilRef )
{
	return create<CD3D11DepthStencilState>(new CD3D11DepthStencilState( desc, stdStencilRef ));
}
