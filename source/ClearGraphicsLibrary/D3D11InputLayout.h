#ifndef D3D11InputLayout_h__
#define D3D11InputLayout_h__

#include "cgl.h"

namespace cgl
{
	namespace core 
	{
		//////////////////////////////////////////////////////////////////////////
		// typedef
		__interface ICGLInstancing;
		typedef std::tr1::shared_ptr<ICGLInstancing> PCGLInstancing;

		//////////////////////////////////////////////////////////////////////////
		// instancing 
		// 
		// decides if an element data is per vertex or per instance 
		__interface CGL_API ICGLInstancing
		{
			void getElementInstancingInfo(LPCSTR semanticName, UINT& semanticIndex, UINT* __out dataStep, UINT* __out instanceBufferSlot);
		};

		//////////////////////////////////////////////////////////////////////////
		// typedefs
		class CD3D11InputLayout;
		typedef CGL_API std::tr1::shared_ptr<CD3D11InputLayout> PD3D11InputLayout;

		//////////////////////////////////////////////////////////////////////////
		// input element
		struct CGL_API CGL_INPUT_ELEMENT_SIGNATURE
		{
			std::string semanticName;
			UINT semanticIndex;
			UINT size;

			bool operator == (CGL_INPUT_ELEMENT_SIGNATURE& rhs)
			{
				return (this->semanticName == rhs.semanticName &&
						this->semanticIndex == rhs.semanticIndex &&
						this->size == rhs.size);
			}
			bool operator != (CGL_INPUT_ELEMENT_SIGNATURE& rhs)
			{
				return !(*this == rhs);
			}
		};

		class CGL_API CD3D11InputLayout : public CGLBase<ID3D11InputLayout>, public cgl::core::CGLTypedBindable<ID3D11InputLayout>
		{
		private:
			PD3D11EffectPass m_pPass;
			ICGLInstancing* m_pInstancingInfo;
			std::vector<CGL_INPUT_ELEMENT_SIGNATURE> m_cglElements;
			UINT m_vertexSize;

		protected:
			HRESULT onRestore();
			void onReset();
			void getDependencies(std::vector<PCGLObject>* pDependencies);
			CD3D11InputLayout(PD3D11EffectPass pPass, ICGLInstancing* pInstancingInfo = nullptr);	

			void bind();
			inline CGL_BIND getBind() { return CGL_BIND_INPUTLAYOUT; }

		public:
			~CD3D11InputLayout();
			static PD3D11InputLayout Create( PD3D11EffectPass pPass, ICGLInstancing* pInstancingInfo = NULL);
	
			inline ICGLInstancing* GetInstancingInfo()							{ return m_pInstancingInfo; }
			inline std::vector<CGL_INPUT_ELEMENT_SIGNATURE>& GetInputElements()	{ return m_cglElements; }
			inline UINT GetElementSize()										{ return m_vertexSize; }

			bool operator == (CD3D11InputLayout& rhs);
		};
	}
}

#endif // D3D11InputLayout_h__