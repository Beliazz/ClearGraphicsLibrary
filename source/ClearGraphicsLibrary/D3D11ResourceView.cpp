#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// CD3D11ShaderResourceView
HRESULT cgl::core::CD3D11ShaderResourceView::onRestore()
{
	ID3D11Resource* pResource = GetResource()->get();
	ZeroMemory(&m_desc, sizeof(m_desc));

	D3D11_RESOURCE_DIMENSION type;
	pResource->GetType(&type);
	switch(type)
	{
		//////////////////////////////////////////////////////////////////////////
		// texture 1D
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			D3D11_TEXTURE1D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			ID3D11Texture1D* pTex = (ID3D11Texture1D*)pResource;
			pTex->GetDesc(&desc);

			m_desc.Format = desc.Format;
			m_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
			m_desc.Texture1D.MipLevels = desc.MipLevels;
			m_desc.Texture1D.MostDetailedMip = 0;

		}

		//////////////////////////////////////////////////////////////////////////
		// texture 2D
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D: 
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			ID3D11Texture2D* pTex = (ID3D11Texture2D*)pResource;
			pTex->GetDesc(&desc);

			m_desc.Format = desc.Format;
			if (desc.ArraySize == 1)
			{				
				m_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				m_desc.Texture2D.MipLevels = desc.MipLevels;
				m_desc.Texture2D.MostDetailedMip = 0;
			}
			else
			{
				m_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				m_desc.Texture2DArray.MipLevels = desc.MipLevels;
				m_desc.Texture2DArray.MostDetailedMip = 0;
				m_desc.Texture2DArray.ArraySize = desc.ArraySize;
				m_desc.Texture2DArray.FirstArraySlice = 0;
			}

		} break;

		//////////////////////////////////////////////////////////////////////////
		// texture 3D
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			D3D11_TEXTURE3D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			ID3D11Texture3D* pTex = (ID3D11Texture3D*)pResource;
			pTex->GetDesc(&desc);

			m_desc.Format = desc.Format;
			m_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
			m_desc.Texture3D.MipLevels = desc.MipLevels;
			m_desc.Texture3D.MostDetailedMip = 0;

		} break;
	}

	return CGLAccess::D3DDevice()->CreateShaderResourceView(GetResource()->get(), &m_desc, ptr());
}
cgl::core::CD3D11ShaderResourceView::CD3D11ShaderResourceView( PD3D11Resource pResource )
	: D3D11ResourceView("CD3D11ShaderResourceView", pResource)
{

}
void cgl::core::CD3D11ShaderResourceView::Bind(PD3D11EffectVariable& pVar)
{	
	pVar->get()->AsShaderResource()->SetResource(get());
}
std::tr1::shared_ptr<cgl::core::CD3D11ShaderResourceView> cgl::core::CD3D11ShaderResourceView::Create( PD3D11Resource pResource )
{
	return create(new CD3D11ShaderResourceView( pResource ));
}
//
// cgl shader resource view collection
cgl::core::CGLShaderResourceViewCollection::CGLShaderResourceViewCollection() : CGLBindCollection("CD3D11SRVCollection") {}
void cgl::core::CGLShaderResourceViewCollection::Bind( cgl::core::CGL_SHADER_STAGE stage )
{
	Bind(stage, 0, GetCollectionSize());
}
void cgl::core::CGLShaderResourceViewCollection::Bind( cgl::core::CGL_SHADER_STAGE stage, UINT startSlot )
{
	Bind(stage, startSlot, GetCollectionSize());
}
void cgl::core::CGLShaderResourceViewCollection::Bind( cgl::core::CGL_SHADER_STAGE stage, UINT startSlot, UINT numViews)
{
	if (startSlot < 0 || startSlot >= GetCollectionSize())
		return;

	if (numViews > GetCollectionSize() )
	{
		numViews = GetCollectionSize();
	}

	switch(stage)
	{
	case cgl::core::CGL_SHADER_STAGE_VERTEX: 	CGLAccess::D3DContext()->VSSetShaderResources(startSlot, numViews, get()); break;
	case cgl::core::CGL_SHADER_STAGE_PIXEL: 	CGLAccess::D3DContext()->PSSetShaderResources(startSlot, numViews, get()); break;
	case cgl::core::CGL_SHADER_STAGE_GEOMETRY:	CGLAccess::D3DContext()->GSSetShaderResources(startSlot, numViews, get()); break;
	case cgl::core::CGL_SHADER_STAGE_HULL: 		CGLAccess::D3DContext()->HSSetShaderResources(startSlot, numViews, get()); break;
	case cgl::core::CGL_SHADER_STAGE_DOMAIN: 	CGLAccess::D3DContext()->DSSetShaderResources(startSlot, numViews, get()); break;
	}
}
cgl::core::PCGLShaderResourceViewCollection cgl::core::CGLShaderResourceViewCollection::Create()
{
	return create(new cgl::core::CGLShaderResourceViewCollection());
}

//////////////////////////////////////////////////////////////////////////
// CD3D11DepthStencilView
HRESULT cgl::core::CD3D11DepthStencilView::onRestore()
{
	ID3D11Resource* pResource = GetResource()->get();
	ZeroMemory(&m_desc, sizeof(m_desc));

	D3D11_RESOURCE_DIMENSION type;
	pResource->GetType(&type);
	switch(type)
	{
		//////////////////////////////////////////////////////////////////////////
		// texture 1D
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			D3D11_TEXTURE1D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			ID3D11Texture1D* pTex = (ID3D11Texture1D*)pResource;
			pTex->GetDesc(&desc);

			m_desc.Format = desc.Format;
			m_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
			m_desc.Texture1D.MipSlice = 0;

		} break;

		//////////////////////////////////////////////////////////////////////////
		// texture 2D
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D: 
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			ID3D11Texture2D* pTex = (ID3D11Texture2D*)pResource;
			pTex->GetDesc(&desc);

			m_desc.Format = desc.Format;
			m_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			m_desc.Texture2D.MipSlice = 0;

		} break;
	}

	return CGLAccess::D3DDevice()->CreateDepthStencilView(GetResource()->get(), &m_desc, ptr());
}
cgl::core::CD3D11DepthStencilView::CD3D11DepthStencilView( PD3D11Resource pResource )
	: D3D11ResourceView("CD3D11DepthStencilView", pResource)
{
}
std::tr1::shared_ptr<cgl::core::CD3D11DepthStencilView> cgl::core::CD3D11DepthStencilView::Create( PD3D11Resource pResource )
{
	return create(new CD3D11DepthStencilView( pResource ));
}
void cgl::core::CD3D11DepthStencilView::Clear(UINT flags, float depth, UINT8 stencil)
{
	CGLAccess::D3DContext()->ClearDepthStencilView(get(), flags, depth, stencil);
}

//////////////////////////////////////////////////////////////////////////
// CD3D11RenderTargetView
HRESULT cgl::core::CD3D11RenderTargetView::onRestore()
{
	HRESULT result = S_FALSE;

	ID3D11Resource* pResource = GetResource()->get();
	ZeroMemory(&m_desc, sizeof(m_desc));

	D3D11_RESOURCE_DIMENSION type;
	pResource->GetType(&type);
	switch(type)
	{
		//////////////////////////////////////////////////////////////////////////
		// texture 1D
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			D3D11_TEXTURE1D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			ID3D11Texture1D* pTex = (ID3D11Texture1D*)pResource;
			pTex->GetDesc(&desc);

			m_desc.Format = desc.Format;
			m_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
			m_desc.Texture1D.MipSlice = 0;

		} break;

		//////////////////////////////////////////////////////////////////////////
		// texture 2D
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D: 
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			ID3D11Texture2D* pTex = (ID3D11Texture2D*)pResource;
			pTex->GetDesc(&desc);

			m_desc.Format = desc.Format;
			m_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			m_desc.Texture2D.MipSlice = 0;

		} break;

		//////////////////////////////////////////////////////////////////////////
		// texture 3D
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			D3D11_TEXTURE3D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			ID3D11Texture3D* pTex = (ID3D11Texture3D*)pResource;
			pTex->GetDesc(&desc);

			m_desc.Format = desc.Format;
			m_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
			m_desc.Texture3D.MipSlice = 0;

		} break;
	}

	return CGLAccess::D3DDevice()->CreateRenderTargetView(GetResource()->get(), &m_desc, ptr());
}
cgl::core::CD3D11RenderTargetView::CD3D11RenderTargetView(PD3D11Resource pResource )
	: D3D11ResourceView("CD3D11RenderTargetView", pResource)
{

}
std::tr1::shared_ptr<cgl::core::CD3D11RenderTargetView> cgl::core::CD3D11RenderTargetView::Create( PD3D11Resource pResource )
{
	return create(new CD3D11RenderTargetView( pResource ));
}
void cgl::core::CD3D11RenderTargetView::Clear( float color[4] )
{
	CGLAccess::D3DContext()->ClearRenderTargetView(get(), color);
}
void cgl::core::CD3D11RenderTargetView::Clear( float r, float g, float b, float a )
{
	float color[] = {r, g, b, a};
	Clear(color);
}
//
// cgl render target view collection
cgl::core::CGLRenderTargetViewCollection::CGLRenderTargetViewCollection()
	: CGLBindCollection("CD3D11RTVCollection"), m_ppSavedCollection(NULL), m_pSavedDepthStencilView(NULL) { }
cgl::core::CGLRenderTargetViewCollection::~CGLRenderTargetViewCollection()
{
	if (m_ppSavedCollection)
	{
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			SAFE_RELEASE(m_ppSavedCollection[i]);

		SAFE_DELETE(m_ppSavedCollection);
		SAFE_RELEASE(m_pSavedDepthStencilView);
	}
}
void cgl::core::CGLRenderTargetViewCollection::Bind( )
{
	Bind(GetCollectionSize());
}
void cgl::core::CGLRenderTargetViewCollection::Bind( UINT numViews )
{
	if (numViews > GetCollectionSize() )
	{
		numViews = GetCollectionSize();
	}

	if (m_pDepthStencilView)
	{
		CGLAccess::D3DContext()->OMSetRenderTargets(numViews, get(), m_pDepthStencilView->get());
	}
	else
	{
		CGLAccess::D3DContext()->OMSetRenderTargets(numViews, get(), NULL);
	}

}
void cgl::core::CGLRenderTargetViewCollection::SetDepthStencilView( cgl::core::PD3D11DepthStencilView pDepthStencilView )
{
	m_pDepthStencilView = pDepthStencilView;
}
cgl::core::PCGLRenderTargetViewCollection cgl::core::CGLRenderTargetViewCollection::Create()
{
	return create(new CGLRenderTargetViewCollection());
}
void cgl::core::CGLRenderTargetViewCollection::Save()
{
	if (m_ppSavedCollection)
	{
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			SAFE_RELEASE(m_ppSavedCollection[i]);

		SAFE_DELETE(m_ppSavedCollection);
		SAFE_RELEASE(m_pSavedDepthStencilView);
	}

	m_ppSavedCollection = new ID3D11RenderTargetView*[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	CGLAccess::D3DContext()->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, m_ppSavedCollection, &m_pSavedDepthStencilView );
}
void cgl::core::CGLRenderTargetViewCollection::Load()
{
	if (m_ppSavedCollection)
	{
		CGLAccess::D3DContext()->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, m_ppSavedCollection, m_pSavedDepthStencilView );
	}
}

