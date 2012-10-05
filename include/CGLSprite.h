#pragma once

#include "cgl.h"
#include <xnamath.h>

namespace cgl
{
	namespace drawing
	{		
		class CGLSprite;
		typedef std::tr1::shared_ptr<CGLSprite> PCGLSprite;

		struct CGLSpriteVertex
		{
			XMFLOAT4 pos;
			XMFLOAT2 tex;

			CGLSpriteVertex()
			{
				pos = XMFLOAT4();
				tex = XMFLOAT2();
			}
			CGLSpriteVertex(XMFLOAT4 pos, XMFLOAT2 tex )
			{
				this->pos = pos;
				this->tex = tex;
			}
		};

		class CGL_API CGLSprite : public cgl::CGLManagerConnector
		{
		private:	
			float m_x;
			float m_y;
			float m_width;
			float m_height;
			float m_rotation;

			bool m_matUpToDate;
			XMFLOAT4X4 m_matWorld;

			UINT m_bbWidth;
			UINT m_bbHeight;

			XMFLOAT4 m_color;

			cgl::PD3D11BackBuffer m_pBackbuffer;
			cgl::PD3D11ShaderResourceView m_pShaderResource;
			cgl::PD3D11RenderTargetView m_pRenderTarget;
			cgl::PD3D11Resource m_pTexture;

			CGLSprite();

		public:
			static PCGLSprite Create();

			void SetX(float x);
			void SetY(float y);
			void AddX(float dx);
			void AddY(float dy);

			void SetWidth(float w);
			void SetHeight(float h);
			void AddWidth(float dw);
			void AddHeight(float dh);

			void SetRotation(float degrees);
			void AddRotation(float dd);
			bool SetTexture(cgl::PD3D11Resource pTex);
			void SetColor(XMFLOAT4 color);

			inline float GetX()							{ return m_x; }
			inline float GetY()							{ return m_y; }
			inline float GetWidth()						{ return m_width; }
			inline float GetHeight()					{ return m_height; }
			inline float GetRotation()					{ return m_rotation; }
			inline XMFLOAT4 GetColor()					{ return m_color; }

			inline cgl::PD3D11Resource& GetTexture()					{ return m_pTexture; }
			inline cgl::PD3D11ShaderResourceView& GetShaderResource()	{ return m_pShaderResource; }
			inline cgl::PD3D11RenderTargetView& GetRenderTarget()		{ return m_pRenderTarget;}

			XMFLOAT4X4 GetWorldMatrix();
			cgl::PD3D11VertexBuffer& GetVertexBuffer();
		};
	}
}



