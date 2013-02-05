#ifndef D3D11RenderStates_h__
#define D3D11RenderStates_h__

#include "cgl.h"

namespace cgl
{
	namespace core
	{
		//////////////////////////////////////////////////////////////////////////
		// rasterizer state
		class CGL_API CGLRasterizerStateBindable : public CGLTypedBindable<ID3D11RasterizerState>
		{
		private:

		protected:
			void bind();
			inline CGL_BIND getBind() { return CGL_BIND_RASTERIZER_STATE; }

		public:
			CGLRasterizerStateBindable(CGLBase<ID3D11RasterizerState>* pRasterizerState) 
				: CGLTypedBindable(pRasterizerState) { }
		};
		class CGL_API CD3D11RasterizerState : public CGLBase<ID3D11RasterizerState>
		{
		private:
			D3D11_RASTERIZER_DESC m_desc;

		protected:
			CD3D11RasterizerState(D3D11_RASTERIZER_DESC desc);
			HRESULT onRestore();
			void onReset();
			void getDependencies(std::vector<PCGLObject>* pDependencies ) { }
			inline bool checkDependency() { return true; }

		public:
			static std::tr1::shared_ptr<CD3D11RasterizerState> Create(D3D11_RASTERIZER_DESC desc);

			void Bind();
		};

		//////////////////////////////////////////////////////////////////////////
		// blend state
		//////////////////////////////////////////////////////////////////////////
		class CGL_API CGLBlendStateBindable : public CGLTypedBindable<ID3D11BlendState>
		{
		protected:
			void bind();
			inline CGL_BIND getBind() { return CGL_BIND_BLEND_STATE; }

		public:
			CGLBlendStateBindable(CGLBase<ID3D11BlendState>* pRasterizerState) 
				: CGLTypedBindable(pRasterizerState) { }

			enum BIND_PARAM
			{
				BIND_PARAM_BLEND_FACTOR_FLOAT4,
				BIND_PARAM_SAMPLE_MASK_UINT
			};
		};
		class CGL_API CD3D11BlendState : public CGLBase<ID3D11BlendState>
		{
		private:
			D3D11_BLEND_DESC m_desc;
			float m_stdBlendFactor[4];
			UINT  m_stdSampleMask;

		protected:
			HRESULT onRestore();
			void onReset();
			void getDependencies(std::vector<PCGLObject>* pDependencies ) { }
			inline bool checkDependency() { return true; }
			CD3D11BlendState(D3D11_BLEND_DESC desc, const float stdBlendFactor[4], UINT stdSampleMask);

		public:
			static std::tr1::shared_ptr<CD3D11BlendState> Create(D3D11_BLEND_DESC desc, const float stdBlendFactor[4], UINT stdSampleMask);

			// bind with std parameter
			void Bind();

			// bind with other parameter
			void Bind(float stdBlendFactor[4], UINT stdSampleMask);

		};

		//////////////////////////////////////////////////////////////////////////
		// depth stencil state
		//////////////////////////////////////////////////////////////////////////
		class CGL_API CD3D11DepthStencilState : public CGLBase<ID3D11DepthStencilState>
		{
		private:
			D3D11_DEPTH_STENCIL_DESC	m_desc;
			UINT8 m_stdStencilRef;

		protected:
			HRESULT onRestore();
			void onReset();
			void getDependencies(std::vector<PCGLObject>* pDependencies ) { }
			CD3D11DepthStencilState(D3D11_DEPTH_STENCIL_DESC desc, UINT8 stdStencilRef);

		public:
			static std::tr1::shared_ptr<CD3D11DepthStencilState> Create(D3D11_DEPTH_STENCIL_DESC desc, UINT8 stdStencilRef);

			// bind with std parameter
			void Bind();

			// bind with other parameter
			void Bind(UINT8 stencilRef);
		};

		//////////////////////////////////////////////////////////////////////////
		// typedefs
		typedef CGL_API std::tr1::shared_ptr<CD3D11RasterizerState> PD3D11RasterizerState;
		typedef CGL_API std::tr1::shared_ptr<CD3D11BlendState> PD3D11BlendState;
		typedef CGL_API std::tr1::shared_ptr<CD3D11DepthStencilState> PD3D11DepthStencilState;
	}
}

#endif // D3D11RenderStates_h__