#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// d3d11 query
cgl::CD3D11Query::CD3D11Query( D3D11_QUERY_DESC desc ) : CGLBase("CD3D11Query"), m_desc(desc)
{

}
HRESULT cgl::CD3D11Query::onRestore()
{
	return getDevice()->GetDevice()->CreateQuery(&m_desc, ptr());
}
void cgl::CD3D11Query::onReset()
{
	comReset((IUnknown**)ptr());
}
void cgl::CD3D11Query::Begin()
{
	getDevice()->GetContext()->Begin(get());
}
void cgl::CD3D11Query::End()
{
	getDevice()->GetContext()->End(get());
}
void cgl::CD3D11Query::GetData( void* pData )
{
	while(S_OK != getDevice()->GetContext()->GetData(get(), 
													 pData, 
													 get()->GetDataSize(),
													 0))
	{  }	
}
std::tr1::shared_ptr<cgl::CD3D11Query> cgl::CD3D11Query::Create( D3D11_QUERY_DESC desc )
{
	return create<CD3D11Query>(new CD3D11Query(desc));
}

