#include "CGLSpriteBatch.h"

cgl::drawing::CGLSpriteBatch::CGLSpriteBatch(cgl::core::PD3D11Effect pEffect)
	: m_pEffect(pEffect)
{
	m_pVertexBuffer = cgl::core::CD3D11VertexBuffer::Create(sizeof(cgl::drawing::CGLSpriteVertex), D3D11_USAGE_IMMUTABLE);
	m_pIndexBuffer = cgl::core::CD3D11IndexBuffer::Create(sizeof(DWORD), D3D11_USAGE_IMMUTABLE);

	m_pTexture = cgl::core::CD3D11EffectVariableFromSemantic::Create(m_pEffect, "TEXTURE");
	m_pWorldMatrix = cgl::core::CD3D11EffectVariableFromSemantic::Create(m_pEffect, "WORLD");
	m_pColor = cgl::core::CD3D11EffectVariableFromSemantic::Create(m_pEffect, "SPRITE_COLOR");
}
cgl::drawing::PCGLSpriteBatch cgl::drawing::CGLSpriteBatch::Create( cgl::core::PD3D11Effect pEffect )
{
	return PCGLSpriteBatch(new CGLSpriteBatch(pEffect));
}

bool cgl::drawing::CGLSpriteBatch::Init()
{
	CGLSpriteVertex vertices[] =
	{
		CGLSpriteVertex(XMFLOAT4( 1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)),
		CGLSpriteVertex(XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)),
		CGLSpriteVertex(XMFLOAT4(-1.0f,  1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)),
		CGLSpriteVertex(XMFLOAT4( 1.0f,  1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f))
	};

	m_pVertexBuffer->SetData((char*)vertices, 4);

	DWORD indices[6] = 
	{
		0, 1, 2,
		0, 2, 3
	};

	m_pIndexBuffer->SetData((char*)indices, 6);

	if (!CGL_RESTORE(m_pVertexBuffer) ||
		!CGL_RESTORE(m_pIndexBuffer))
	{
		return false;
	}

	m_pTechnique = cgl::core::CD3D11EffectTechniqueFromIndex::Create(m_pEffect, 0);
	if (!CGL_RESTORE(m_pTechnique))
		return false;

	for (UINT i = 0; i < m_pTechnique->Passes(); i++)
	{
		m_passes.push_back(cgl::core::CD3D11EffectPassFromIndex::Create(m_pTechnique, i));
		if (!CGL_RESTORE(m_passes[i]))
			return false;
	}

	m_pInputLayout = cgl::core::CD3D11InputLayout::Create(m_passes[0]);
	if (!CGL_RESTORE(m_pInputLayout))
		return false;


	if (!CGL_RESTORE(m_pWorldMatrix) ||
		!CGL_RESTORE(m_pTexture) ||
		!CGL_RESTORE(m_pColor))
	{
		return false;
	}

	CD3D11_TEXTURE2D_DESC desc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, 1, 1, 1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	cgl::core::PD3D11Resource defaultTex = cgl::core::CD3D11Texture2DBlank::Create(desc);
	m_pDefaultTextureRTV = cgl::core::CD3D11RenderTargetView::Create(defaultTex);
	m_pDefaultTextureSRV = cgl::core::CD3D11ShaderResourceView::Create(defaultTex);

	if (!CGL_RESTORE(defaultTex)||
		!CGL_RESTORE(m_pDefaultTextureSRV) ||
		!CGL_RESTORE(m_pDefaultTextureRTV) )
	{
		return false;
	}

	m_pDefaultTextureRTV->Clear(1.0f, 1.0f, 1.0f, 1.0f);

	return true;
}
void cgl::drawing::CGLSpriteBatch::AddSprite( cgl::drawing::PCGLSprite pSprite )
{
	m_sprites.push_back(pSprite);
}
void cgl::drawing::CGLSpriteBatch::RemoveSprite( UINT index )
{
	m_sprites.erase(m_sprites.begin() + index);
}
void cgl::drawing::CGLSpriteBatch::ResetData()
{
	m_sprites.clear();
}
void cgl::drawing::CGLSpriteBatch::RemoveLastSprite()
{
	m_sprites.pop_back();
}
void cgl::drawing::CGLSpriteBatch::Render()
{
	m_pInputLayout->Bind();
	m_pVertexBuffer->Bind();
	m_pIndexBuffer->Bind();

	cgl::core::CGLAccess::D3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for(UINT i = 0; i < m_sprites.size(); i++)
	{
		FLOAT* col = (float*)&m_sprites[i]->GetColor();

		if (m_sprites[i]->GetShaderResource())
		{
			m_pTexture->get()->AsShaderResource()->SetResource(m_sprites[i]->GetShaderResource()->get());
		}
		else
		{
			m_pTexture->get()->AsShaderResource()->SetResource(m_pDefaultTextureSRV->get());
		}

		m_pColor->get()->AsVector()->SetFloatVector((float*)&m_sprites[i]->GetColor());
		m_pWorldMatrix->get()->AsMatrix()->SetMatrix((float*)&m_sprites[i]->GetWorldMatrix().m);

		for (UINT j = 0; j < m_passes.size(); j++)
		{
			m_passes[0]->Apply();
			m_pIndexBuffer->Draw();
		}
	}

}

