#include "CGLLabel.h"

cgl::drawing::CGLLabel::CGLLabel( PD3D11Effect pEffect, int width, int height ) 
	: CGLObject("CGLLabel"), m_pEffect(pEffect)
{
	m_pSprite = CGLSprite::Create();

	m_pSprite->SetWidth((float)width);
	m_pSprite->SetHeight((float)height);
	
	m_pSpriteBatch = CGLSpriteBatch::Create(m_pEffect);
	m_pSpriteBatch->AddSprite(m_pSprite);
}

HRESULT cgl::drawing::CGLLabel::onRestore()
{
	CD3D11_TEXTURE2D_DESC desc;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = NULL;
	desc.MiscFlags = NULL;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width = (UINT)m_pSprite->GetWidth();
	desc.Height = (UINT)m_pSprite->GetHeight();
	desc.Usage = D3D11_USAGE_DEFAULT;
	cgl::PD3D11Resource pRes = cgl::CD3D11Texture2DBlank::Create(desc);

	if (!m_pSprite->SetTexture(pRes))
		return E_FAIL;

	m_pRenderTargetCollection = cgl::CGLRenderTargetViewCollection::Create();
	m_pRenderTargetCollection->Add(m_pSprite->GetRenderTarget(), 0);
	if (!CGL_RESTORE(m_pRenderTargetCollection))
		return false;

	float col[] = { 0.05f, 0.15f, 0.2f, 0.7f };
	m_pSprite->GetRenderTarget()->Clear(col);

	if (!m_pSpriteBatch->Init())
		return false;

	return true;
}
void cgl::drawing::CGLLabel::onReset()
{
	m_pSpriteBatch->ResetData();
}

void cgl::drawing::CGLLabel::Render()
{
	if (m_bChanged)
	{	
		ID3D11ShaderResourceView* tmp[D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {0};
		mgr()->GetDevice()->GetContext()->PSSetShaderResources(0, D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, tmp);

		m_pRenderTargetCollection->Save();
		m_pRenderTargetCollection->Bind();

		float col[] = { 0.05f, 0.15f, 0.2f, 0.7f };
		m_pSprite->GetRenderTarget()->Clear(col);

		RenderText(m_text);

		m_pRenderTargetCollection->Load();

		m_bChanged = false;
	}

	m_pSpriteBatch->Render();
}

void cgl::drawing::CGLLabel::SetX( float x )
{
	m_pSprite->SetX(x);
}
void cgl::drawing::CGLLabel::SetY( float y )
{
	m_pSprite->SetY(y);
}

float cgl::drawing::CGLLabel::GetX()
{
	return m_pSprite->GetX();
}

float cgl::drawing::CGLLabel::GetY()
{
	return m_pSprite->GetY();
}

float cgl::drawing::CGLLabel::GetWidth()
{
	return m_pSprite->GetWidth();
}

float cgl::drawing::CGLLabel::GetHeight()
{
	return m_pSprite->GetHeight();
}

void cgl::drawing::CGLLabel::SetText( std::string text )
{
	if (text != m_text)
	{
		m_text = text;
		m_bChanged = true;
	}
}
