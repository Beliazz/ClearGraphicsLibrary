#pragma once

#include "cgl.h"
#include "CGLSprite.h"
#include <xnamath.h>

namespace cgl
{
	namespace drawing
	{
		class CGLSpriteBatch;
		typedef std::tr1::shared_ptr<CGLSpriteBatch> PCGLSpriteBatch;

		class CGLSprite;
		class CGLSpriteBatch
		{
		private:
			std::vector<std::tr1::shared_ptr<CGLSprite>> m_sprites;

			cgl::PD3D11Effect m_pEffect;
			cgl::PD3D11EffectTechnique m_pTechnique;
			cgl::PD3D11InputLayout m_pInputLayout;
			std::vector<cgl::PD3D11EffectPass> m_passes;

			cgl::PD3D11VertexBuffer m_pVertexBuffer;
			cgl::PD3D11IndexBuffer  m_pIndexBuffer;

			cgl::PD3D11EffectVariable m_pTexture;
			cgl::PD3D11EffectVariable m_pColor;
			cgl::PD3D11EffectVariable m_pWorldMatrix;

			cgl::PD3D11RenderTargetView m_pDefaultTextureRTV;
			cgl::PD3D11ShaderResourceView m_pDefaultTextureSRV;

			CGLSpriteBatch(cgl::PD3D11Effect pEffect);
			
		public:
			static PCGLSpriteBatch Create(cgl::PD3D11Effect pEffect);

			bool Init();
			void Exit();

			void AddSprite(std::tr1::shared_ptr<CGLSprite> pSprite);
			void RemoveSprite(UINT index);
			void RemoveLastSprite();
			void ResetData();

			void Render();
		};

	}
}




