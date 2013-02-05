#ifndef InputBuffer_h__
#define InputBuffer_h__

#include "cgl.h"

namespace cgl
{
	namespace util
	{
		//////////////////////////////////////////////////////////////////////////
		// typedef
		class CGLInputBuffer;
		typedef CGL_API std::tr1::shared_ptr<CGLInputBuffer>	PCGLInputBuffer;

		//////////////////////////////////////////////////////////////////////////
		// d3d11 input buffer
		class CGL_API CGLInputBuffer : public cgl::core::CGLBase<ID3D11Buffer>, cgl::core::CGLTypedBindable<ID3D11Buffer>
		{	
		private:
			D3D11_BUFFER_DESC m_desc;
			cgl::core::PD3D11InputLayout m_pLayout;
			ICGLInputBufferDataProvider* m_pDataProvider;

		protected:
			HRESULT onRestore();
			void onReset();
			void getDependencies(std::vector<cgl::core::PCGLObject>* pDependencies);
			CGLInputBuffer(cgl::core::PD3D11InputLayout pLayout, ICGLInputBufferDataProvider* pDataProvider, D3D11_USAGE usage, DWORD cpuAccessFlags);
	
			void onParamUpdate();

			 cgl::core::CGL_BIND getBind() { return cgl::core::CGL_BIND_VERTEXBUFFER; }
			 void bind();

		public:
			static PCGLInputBuffer Create(cgl::core::PD3D11InputLayout pLayout, ICGLInputBufferDataProvider* pDataProvider, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, DWORD cpuAccessFlags = 0);

			bool Update();
			void Draw();
			void Draw(UINT offset);
			void Draw(UINT offset, UINT count);

			inline UINT GetStride() { return m_pLayout->GetElementSize(); }
			inline bool IsValid()	{ return m_pDataProvider->isValid(); }

			enum BIND_PARAM
			{
				BIND_PARAM_SLOT_UINT,
				BIND_PARAM_OFFSET_UINT,
				BIND_PARAM_STRIDE_UINT
			};
		};
	}
}

#endif // InputBuffer_h__

