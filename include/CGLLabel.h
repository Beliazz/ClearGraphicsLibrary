#pragma once

#include "cgl.h"

namespace cgl
{
	namespace drawing
	{
		class CGLLabel;
		typedef std::tr1::shared_ptr<CGLLabel> PCGLLabel;

		class CGLSprite;
		class CGLSpriteBatch;

		class CGL_API CGLLabel : public cgl::core::CGLObject
		{
		private:
			std::tr1::shared_ptr<CGLSprite>			m_pSprite;
			std::tr1::shared_ptr<CGLSpriteBatch>	m_pSpriteBatch;
			cgl::core::PD3D11Effect					m_pEffect;
			std::string		m_text;
			cgl::core::PCGLRenderTargetViewCollection m_pRenderTargetCollection;
			bool m_bChanged;

		protected:
			virtual void onReset();
			virtual HRESULT onRestore();
			virtual void getDependencies( std::vector<cgl::core::PCGLObject>* pDependencies ) { pDependencies->push_back(m_pEffect); }

			virtual void RenderText(std::string text) PURE;

			CGLLabel(cgl::core::PD3D11Effect pEffect, cgl::core::PD3D11BackBuffer pBackBuffer, int width, int height);

		public:
			void SetX(float x);
			void SetY(float y);

			void SetText(std::string text);

			float GetX();
			float GetY();
			float GetWidth();
			float GetHeight();

			void Render();
		};
	}
}



