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

		class CGL_API CGLLabel : public CGLObject
		{
		private:
			std::tr1::shared_ptr<CGLSprite>			m_pSprite;
			std::tr1::shared_ptr<CGLSpriteBatch>	m_pSpriteBatch;
			PD3D11Effect	m_pEffect;
			std::wstring	m_text;
			cgl::PCGLRenderTargetViewCollection m_pRenderTargetCollection;
			bool m_bChanged;

		protected:
			virtual void onReset();
			virtual HRESULT onRestore();
			virtual void getDependencies( std::vector<PCGLObject>* pDependencies ) { pDependencies->push_back(m_pEffect); }

			virtual void RenderText(std::wstring text) PURE;

			CGLLabel(PD3D11Effect pEffect, int width, int height);
		public:
			void SetX(float x);
			void SetY(float y);
			void SetText(std::wstring text);

			float GetX();
			float GetY();
			float GetWidth();
			float GetHeight();

			void Render();
		};
	}
}



