#ifndef D3D11Device_h__
#define D3D11Device_h__

#include "cgl.h"

namespace cgl
{
	namespace core
	{

		//////////////////////////////////////////////////////////////////////////
		// typedef
		class CD3D11Device;
		typedef CGL_API std::tr1::shared_ptr<CD3D11Device>  PD3D11Device;

		//////////////////////////////////////////////////////////////////////////
		// d3d11 device class		
		class CGL_API CD3D11Device : public CGLBase<ID3D11Device>
		{
		friend class CGLManager;

		protected:	
			PDXGIAdapter m_pAdapter;
			D3D_FEATURE_LEVEL m_featureLevel;
			UINT m_flags;
			std::vector<UINT> m_bindRegistry;

			CD3D11Device(PDXGIAdapter pAdapter, UINT flags);

			void onReset();
			HRESULT onRestore();
			void getDependencies(std::vector<PCGLObject>* pDependencies );

		public:
			static PD3D11Device Create(PDXGIAdapter pAdapter, UINT flags = 0);

			void Bind(CGLBindable* pBindable);

			inline UINT GetCurrentlyBoundObjectId(CGL_BIND type) { return m_bindRegistry[static_cast<int>(type)]; }
			inline D3D_FEATURE_LEVEL GetFeatureLevel() { return m_featureLevel; }
		};
	}
}

#endif // D3D11Device_h__
