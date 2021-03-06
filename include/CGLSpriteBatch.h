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
		class CGL_API CGLSpriteBatch
		{
		private:
			std::vector<std::tr1::shared_ptr<CGLSprite>> m_sprites;

			cgl::core::PD3D11Effect m_pEffect;
			cgl::core::PD3D11EffectTechnique m_pTechnique;
			cgl::core::PD3D11InputLayout m_pInputLayout;
			std::vector<cgl::core::PD3D11EffectPass> m_passes;

			cgl::core::PD3D11VertexBuffer m_pVertexBuffer;
			cgl::core::PD3D11IndexBuffer  m_pIndexBuffer;

			cgl::core::PD3D11EffectVariable m_pTexture;
			cgl::core::PD3D11EffectVariable m_pColor;
			cgl::core::PD3D11EffectVariable m_pWorldMatrix;

			cgl::core::PD3D11RenderTargetView m_pDefaultTextureRTV;
			cgl::core::PD3D11ShaderResourceView m_pDefaultTextureSRV;

			CGLSpriteBatch(cgl::core::PD3D11Effect pEffect);
			
		public:
			static PCGLSpriteBatch Create(cgl::core::PD3D11Effect pEffect);

			bool Init();

			void AddSprite(std::tr1::shared_ptr<CGLSprite> pSprite);
			void RemoveSprite(UINT index);
			void RemoveLastSprite();
			void ResetData();

			void Render();
		};

	}
}




