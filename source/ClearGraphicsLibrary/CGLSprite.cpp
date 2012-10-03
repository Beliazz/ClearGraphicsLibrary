#include "CGLSprite.h"

using namespace cgl;
using namespace cgl::drawing;

CGLSprite::CGLSprite()
{
	m_pBackbuffer = cgl::CD3D11BackBuffer::Create();
	m_pBackbuffer->restore();
	m_matUpToDate = false;

	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;
	m_rotation = 0;
	m_bbHeight = 0;
	m_bbWidth = 0; 

	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	CD3D11_TEXTURE2D_DESC desc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, 2, 2, 1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	m_pTexture = cgl::CD3D11Texture2DBlank::Create(desc);
	m_pShaderResource = cgl::CD3D11ShaderResourceView::Create(m_pTexture);
	m_pRenderTarget = cgl::CD3D11RenderTargetView::Create(m_pTexture);

	CGL_RESTORE(m_pShaderResource);
	CGL_RESTORE(m_pRenderTarget);

	m_pRenderTarget->Clear(1.0f, 1.0f, 1.0f, 1.0f);
}

PCGLSprite CGLSprite::Create()
{
	return PCGLSprite(new CGLSprite());
}

void CGLSprite::SetWidth( float w )
{
	m_width = w;

	// 	D3D11_TEXTURE2D_DESC desc;
	// 	((ID3D11Texture2D*)m_pBackbuffer->get())->GetDesc(&desc);
	//	m_bbWidth = desc.Width;

	D3D11_VIEWPORT vp;
	UINT count = 1;
	mgr()->GetDevice()->GetContext()->RSGetViewports(&count, &vp);
	m_bbWidth = (UINT)vp.Width;

	m_matUpToDate = false;
}
void CGLSprite::SetHeight( float h )
{
	m_height = h;

	// 	D3D11_TEXTURE2D_DESC desc;
	// 	((ID3D11Texture2D*)m_pBackbuffer->get())->GetDesc(&desc);
	// 
	// 	m_bbHeight = desc.Height;

	D3D11_VIEWPORT vp;
	UINT count = 1;
	mgr()->GetDevice()->GetContext()->RSGetViewports(&count, &vp);
	m_bbHeight =  (UINT)vp.Height;

	m_matUpToDate = false;
}
void CGLSprite::AddWidth( float dw )
{
	SetWidth(m_width + dw);
}
void CGLSprite::AddHeight( float dh )
{
	SetHeight(m_height + dh);
}

void CGLSprite::SetX( float x )
{
	m_x = x;
	m_matUpToDate = false;
}
void CGLSprite::SetY( float y )
{
	m_y = y;
	m_matUpToDate = false;
}

void CGLSprite::AddX( float dx )
{
	SetX(m_x + dx);
}
void CGLSprite::AddY( float dy )
{
	SetY(m_y + dy);
}

void CGLSprite::SetRotation( float degrees )
{
	m_rotation = degrees;
	m_matUpToDate = false;
}
void CGLSprite::AddRotation( float dd )
{
	SetRotation(m_rotation + dd);
}

XMFLOAT4X4 CGLSprite::GetWorldMatrix()
{
	if (m_matUpToDate)
		return m_matWorld;

	D3D11_VIEWPORT vp;
	UINT count = 1;
	mgr()->GetDevice()->GetContext()->RSGetViewports(&count, &vp);
	m_bbHeight = (UINT)vp.Height;
	m_bbWidth = (UINT)vp.Width;

	float x =  ((FLOAT)m_x - m_bbWidth  / 2 + m_width  / 2) / (m_bbWidth / 2.0f);
	float y = -((FLOAT)m_y - m_bbHeight / 2 + m_height / 2) / (m_bbHeight / 2.0f);

	XMMATRIX scale = XMMatrixScaling(m_width / m_bbWidth, m_height / m_bbHeight, 1.0f);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, m_rotation * XM_PI / 180.0f);
	XMMATRIX translationForth = XMMatrixTranslation(x, y, 0);
	XMMATRIX translationBack = XMMatrixTranslation(-x, -y, 0);

	XMMATRIX world = XMMatrixMultiply(scale, translationForth);
	world = XMMatrixMultiply(world, translationBack);
	world = XMMatrixMultiply(world, rotation);
	world = XMMatrixMultiply(world, translationForth);

	XMStoreFloat4x4(&m_matWorld, world);

	m_matUpToDate = true;

	return m_matWorld;
}

void CGLSprite::SetColor( XMFLOAT4 color )
{
	m_color = color;
}
bool CGLSprite::SetTexture( cgl::PD3D11Resource pTex )
{
	m_pTexture = pTex;

	m_pShaderResource = cgl::CD3D11ShaderResourceView::Create(pTex);
	m_pRenderTarget = cgl::CD3D11RenderTargetView::Create(pTex);

	if(!CGL_RESTORE(m_pRenderTarget)||
		!CGL_RESTORE(m_pShaderResource))
	{
		m_pTexture = nullptr;
		m_pShaderResource = nullptr;
		m_pRenderTarget = nullptr;

		return false;
	}

	return true;
}