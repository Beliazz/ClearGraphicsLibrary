#pragma once
#include "cgl.h"

namespace cgl
{
	namespace core
	{
		#pragma pack (push)
		struct CGLSwapChainDesc
		{
		public:
			UINT bufferDescIndex;
			UINT bufferCount;
			DXGI_USAGE bufferUsage;
			DXGI_SWAP_EFFECT swapEffect;
			UINT samplingCount;
			UINT samplingQuality;
			
			UINT flags;
			bool windowed;

			char unused[3];

			CGLSwapChainDesc(UINT bufferDescIndex = 0,
							 bool windowed = true,
							 UINT bufferCount = 1, 
							 DXGI_USAGE bufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
							 DXGI_SWAP_EFFECT swapEffect = DXGI_SWAP_EFFECT_DISCARD,
							 UINT samplingQuality = 0,
							 UINT samplingCount = 1,
							 UINT flags = NULL)
			{
				this->bufferDescIndex = bufferDescIndex;
				this->bufferCount = bufferCount;
				this->flags = flags;
				this->bufferUsage = bufferUsage;
				this->samplingQuality = samplingQuality;
				this->samplingCount = samplingCount;
				this->swapEffect = swapEffect;
				this->windowed = windowed;
			}
		};
		#pragma pack (pop)

		typedef std::tr1::shared_ptr<class CDXGISwapChain> PDXGISwapChain;
		class CGL_API CDXGISwapChain : public cgl::core::CGLBase<IDXGISwapChain>
		{
		private:
			PDXGIOutput m_pOutput;
			cgl::util::PCGLWindow m_pWindow;
			CGLSwapChainDesc m_desc;

		protected:
			CDXGISwapChain(cgl::util::PCGLWindow pWindow, PDXGIOutput pOutput, CGLSwapChainDesc desc);

			HRESULT onRestore();
			void onReset();
			void getDependencies(std::vector<PCGLObject>* pDependencies);

		public:
			static PDXGISwapChain Create(cgl::util::PCGLWindow pWindow, PDXGIOutput pOutput, CGLSwapChainDesc desc);
			~CDXGISwapChain(void);
		};
	}
}


