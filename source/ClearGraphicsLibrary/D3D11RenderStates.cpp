#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// rasterizer state
cgl::core::CD3D11RasterizerState::CD3D11RasterizerState(D3D11_RASTERIZER_DESC desc) : CGLBase("CD3D11RasterizerState"), m_desc(desc)
{

}
HRESULT cgl::core::CD3D11RasterizerState::onRestore()
{
	return CGLAccess::D3DDevice()->CreateRasterizerState(&m_desc, ptr());
}
void cgl::core::CD3D11RasterizerState::Bind()
{
	CGLAccess::D3DContext()->RSSetState(get());
}
std::tr1::shared_ptr<cgl::core::CD3D11RasterizerState> cgl::core::CD3D11RasterizerState::Create( D3D11_RASTERIZER_DESC desc )
{
	return create<CD3D11RasterizerState>(new CD3D11RasterizerState(desc));
}
void cgl::core::CD3D11RasterizerState::onReset()
{
	comReset((IUnknown**)ptr());
}
//
// cgl rasterizer state bindable
void cgl::core::CGLRasterizerStateBindable::bind()
{
	CGLAccess::D3DContext()->RSSetState(m_pObject__->get());
}

//////////////////////////////////////////////////////////////////////////
// blend state
cgl::core::CD3D11BlendState::CD3D11BlendState(D3D11_BLEND_DESC desc, const float stdBlendFactor[4], UINT stdSampleMask) : CGLBase("CD3D11BlendState"), m_desc(desc), m_stdSampleMask(stdSampleMask)
{
	memcpy(&m_stdBlendFactor[0], &stdBlendFactor[0], 4 * sizeof(float) );
}
HRESULT cgl::core::CD3D11BlendState::onRestore()
{
	return CGLAccess::D3DDevice()->CreateBlendState(&m_desc, ptr());
}
void cgl::core::CD3D11BlendState::onReset()
{
	comReset((IUnknown**)ptr());
}
void cgl::core::CD3D11BlendState::Bind()
{
	Bind(m_stdBlendFactor, m_stdSampleMask);
}
void cgl::core::CD3D11BlendState::Bind( float blendFactor[4], UINT sampleMask )
{
	CGLAccess::D3DContext()->OMSetBlendState(get(), blendFactor, sampleMask);
}
std::tr1::shared_ptr<cgl::core::CD3D11BlendState> cgl::core::CD3D11BlendState::Create( D3D11_BLEND_DESC desc, const float stdBlendFactor[4], UINT stdSampleMask )
{
	return create(new CD3D11BlendState( desc, stdBlendFactor, stdSampleMask ));
}
//
// cgl blend state bindable
void cgl::core::CGLBlendStateBindable::bind()
{
	CGLAccess::D3DContext()->OMSetBlendState(m_pObject__->get(), GetParam<float*>(BIND_PARAM_BLEND_FACTOR_FLOAT4), GetParam<UINT>(BIND_PARAM_SAMPLE_MASK_UINT));
}

//////////////////////////////////////////////////////////////////////////
// depth stencil state
cgl::core::CD3D11DepthStencilState::CD3D11DepthStencilState(D3D11_DEPTH_STENCIL_DESC desc, UINT8 stdStencilRef) : CGLBase("CD3D11DepthStencilState"), m_desc(desc)  { }
HRESULT cgl::core::CD3D11DepthStencilState::onRestore()
{
	return CGLAccess::D3DDevice()->CreateDepthStencilState(&m_desc, ptr());
}
void cgl::core::CD3D11DepthStencilState::onReset()
{
	comReset((IUnknown**)ptr());
}
void cgl::core::CD3D11DepthStencilState::Bind()
{
	Bind(m_stdStencilRef);
}
void cgl::core::CD3D11DepthStencilState::Bind( UINT8 stencilRef )
{
	CGLAccess::D3DContext()->OMSetDepthStencilState(get(), stencilRef);
}
std::tr1::shared_ptr<cgl::core::CD3D11DepthStencilState> cgl::core::CD3D11DepthStencilState::Create( D3D11_DEPTH_STENCIL_DESC desc, UINT8 stdStencilRef )
{
	return create(new CD3D11DepthStencilState( desc, stdStencilRef ));
}
