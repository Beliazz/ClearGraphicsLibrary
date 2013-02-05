#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// d3d11 query
cgl::core::CD3D11Query::CD3D11Query( D3D11_QUERY_DESC desc ) : CGLBase("CD3D11Query"), m_desc(desc)
{

}
HRESULT cgl::core::CD3D11Query::onRestore()
{
	return CGLAccess::D3DDevice()->CreateQuery(&m_desc, ptr());
}
void cgl::core::CD3D11Query::onReset()
{
	comReset((IUnknown**)ptr());
}
void cgl::core::CD3D11Query::Begin()
{
	CGLAccess::D3DContext()->Begin(get());
}
void cgl::core::CD3D11Query::End()
{
	CGLAccess::D3DContext()->End(get());
}
void cgl::core::CD3D11Query::GetData( void* pData )
{
	while(S_OK != CGLAccess::D3DContext()->GetData(get(), 
									 pData, 
									 get()->GetDataSize(),
									 0))
	{  }	
}
std::tr1::shared_ptr<cgl::core::CD3D11Query> cgl::core::CD3D11Query::Create( D3D11_QUERY_DESC desc )
{
	return create(new CD3D11Query(desc));
}

