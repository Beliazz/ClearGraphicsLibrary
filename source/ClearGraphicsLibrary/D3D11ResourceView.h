#ifndef D3D11ShaderResourceView_h__
#define D3D11ShaderResourceView_h__

#include "cgl.h"

namespace cgl
{
	namespace core
	{
		//////////////////////////////////////////////////////////////////////////
		// typedefs
		class CD3D11ShaderResourceView;
		class CD3D11RenderTargetView;
		class CD3D11DepthStencilView;
		typedef CGL_API std::tr1::shared_ptr<CD3D11ShaderResourceView> PD3D11ShaderResourceView; 
		typedef CGL_API std::tr1::shared_ptr<CD3D11DepthStencilView> PD3D11DepthStencilView; 
		typedef CGL_API std::tr1::shared_ptr<CD3D11RenderTargetView> PD3D11RenderTargetView; 

		//////////////////////////////////////////////////////////////////////////
		// D3D11ResourceView
		template <class T>
		class CGL_API D3D11ResourceView : public CGLBase<T>
		{
		private:
			PD3D11Resource m_pResource;
	
		protected:
			inline void onReset()
			{
				comReset((IUnknown**)ptr());
			}
			inline void getDependencies(std::vector<PCGLObject>* pDependencies )
			{
				pDependencies->push_back(m_pResource);
			}

		public:
			D3D11ResourceView(std::string className, PD3D11Resource pResource)
				: CGLBase(className), m_pResource(pResource)
			{

			}
			virtual ~D3D11ResourceView() { onReset(); }
	
			inline PD3D11Resource GetResource() {return m_pResource;}
		};

		//////////////////////////////////////////////////////////////////////////
		// CD3D11ShaderResourceView
		class CGL_API CGLShaderResourceViewCollection : public CGLBindCollection<ID3D11ShaderResourceView>
		{
		protected:
			void getDependencies(std::vector<cgl::core::PCGLObject>* pDependencies ) { }
			CGLShaderResourceViewCollection();

		public:
			static PCGLShaderResourceViewCollection Create();

			// set all views starting at slot 0
			void Bind(cgl::core::CGL_SHADER_STAGE stage);

			// set all views starting at the specified slot
			void Bind(cgl::core::CGL_SHADER_STAGE stage, UINT startSlot);

			// set a custom number of views
			// if the number is greater than the current collection size
			// it numViews will be changed to the collection size
			void Bind(cgl::core::CGL_SHADER_STAGE stage, UINT startSlot, UINT numViews);
		};
		class CGL_API CD3D11ShaderResourceView : public D3D11ResourceView<ID3D11ShaderResourceView>
		{
		private:
			D3D11_SHADER_RESOURCE_VIEW_DESC m_desc;

		protected:
			CD3D11ShaderResourceView(PD3D11Resource pResource);
			HRESULT onRestore();

		public:
			static PD3D11ShaderResourceView Create(PD3D11Resource pResource);
			void Bind(PD3D11EffectVariable& pVar);
		};

		//////////////////////////////////////////////////////////////////////////
		// CD3D11DepthStencilView
		class CGL_API CD3D11DepthStencilView : public D3D11ResourceView<ID3D11DepthStencilView>
		{
		private:
			D3D11_DEPTH_STENCIL_VIEW_DESC m_desc;

		protected:
			CD3D11DepthStencilView(PD3D11Resource pResource);
			HRESULT onRestore();

		public:
			static PD3D11DepthStencilView Create(PD3D11Resource pResource);
			void Clear(UINT flags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, float depth = 0.0f, UINT8 stencil = 0);
		};

		//////////////////////////////////////////////////////////////////////////
		// CD3D11RenderTargetView
		class CGL_API CGLRenderTargetViewCollection : public CGLBindCollection<ID3D11RenderTargetView>
		{
		private:
			cgl::core::PD3D11DepthStencilView m_pDepthStencilView;
			ID3D11RenderTargetView** m_ppSavedCollection;
			ID3D11DepthStencilView* m_pSavedDepthStencilView;

		protected:
			void getDependencies(std::vector<cgl::core::PCGLObject>* pDependencies ) { }
			CGLRenderTargetViewCollection();

		public:
			~CGLRenderTargetViewCollection();
			static PCGLRenderTargetViewCollection Create();

			// set depth stencil view
			void SetDepthStencilView(cgl::core::PD3D11DepthStencilView pDepthStencilView);

			// set all views
			void Bind();

			// set a custom number of views
			// if the number is greater than the current collection size
			// it numViews will be changed to the collection size
			void Bind(UINT numViews);

			void Save();
			void Load();
		};
		class CGL_API CD3D11RenderTargetView : public D3D11ResourceView<ID3D11RenderTargetView>
		{
		private:
			D3D11_RENDER_TARGET_VIEW_DESC m_desc;

		protected:
			CD3D11RenderTargetView(PD3D11Resource pResource);
			HRESULT onRestore();

		public:
			static PD3D11RenderTargetView Create(PD3D11Resource pResource);

			void Clear(float color[4]);
			void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f);
		};
	}
}

#endif // D3D11ShaderResourceView_h__
