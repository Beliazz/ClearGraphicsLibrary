#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// CD3D11ShaderResourceView
HRESULT cgl::CD3D11ShaderResourceView::onRestore()
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

	return getDevice()->GetDevice()->CreateShaderResourceView(GetResource()->get(), &m_desc, ptr());
}
cgl::CD3D11ShaderResourceView::CD3D11ShaderResourceView( PD3D11Resource pResource )
	: D3D11ResourceView("CD3D11ShaderResourceView", pResource)
{

}
void cgl::CD3D11ShaderResourceView::Bind(PD3D11EffectVariable& pVar)
{	
	pVar->get()->AsShaderResource()->SetResource(get());
}
std::tr1::shared_ptr<cgl::CD3D11ShaderResourceView> cgl::CD3D11ShaderResourceView::Create( PD3D11Resource pResource )
{
	return create<CD3D11ShaderResourceView>(new CD3D11ShaderResourceView( pResource ));
}

//////////////////////////////////////////////////////////////////////////
// CD3D11DepthStencilView
HRESULT cgl::CD3D11DepthStencilView::onRestore()
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

	return getDevice()->GetDevice()->CreateDepthStencilView(GetResource()->get(), &m_desc, ptr());
}
cgl::CD3D11DepthStencilView::CD3D11DepthStencilView( PD3D11Resource pResource )
	: D3D11ResourceView("CD3D11DepthStencilView", pResource)
{
}
std::tr1::shared_ptr<cgl::CD3D11DepthStencilView> cgl::CD3D11DepthStencilView::Create( PD3D11Resource pResource )
{
	return create<CD3D11DepthStencilView>(new CD3D11DepthStencilView( pResource ));
}
void cgl::CD3D11DepthStencilView::Clear(UINT flags, float depth, UINT8 stencil)
{
	getDevice()->GetContext()->ClearDepthStencilView(get(), flags, depth, stencil);
}

//////////////////////////////////////////////////////////////////////////
// CD3D11RenderTargetView
HRESULT cgl::CD3D11RenderTargetView::onRestore()
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

	return getDevice()->GetDevice()->CreateRenderTargetView(GetResource()->get(), &m_desc, ptr());
}
cgl::CD3D11RenderTargetView::CD3D11RenderTargetView(PD3D11Resource pResource )
	: D3D11ResourceView("CD3D11RenderTargetView", pResource)
{

}
std::tr1::shared_ptr<cgl::CD3D11RenderTargetView> cgl::CD3D11RenderTargetView::Create( PD3D11Resource pResource )
{
	return create<CD3D11RenderTargetView>(new CD3D11RenderTargetView( pResource ));
}
void cgl::CD3D11RenderTargetView::Clear( float color[4] )
{
	getDevice()->GetContext()->ClearRenderTargetView(get(), color);
}
void cgl::CD3D11RenderTargetView::Clear( float r, float g, float b, float a )
{
	float color[] = {r, g, b, a};
	Clear(color);
}


