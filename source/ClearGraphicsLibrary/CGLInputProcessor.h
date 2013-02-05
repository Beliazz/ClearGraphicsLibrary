#ifndef CGLInputProcessor_h__
#define CGLInputProcessor_h__

#include "cgl.h"

namespace cgl 
{
	namespace util
	{
		//////////////////////////////////////////////////////////////////////////
		// typedef
		class CGLInputDataProcessor;
		typedef CGL_API std::tr1::shared_ptr<CGLInputDataProcessor>	PCGLInputDataProcessor;

		//////////////////////////////////////////////////////////////////////////
		// cgl input buffer data provider
		__interface CGL_API ICGLInputBufferDataProvider	
		{
			UINT getElementCount();
			UINT getDataSize();
			UINT getElementSize();
			char* getData();
			bool isCompatible(cgl::core::PD3D11InputLayout& pLayoutToMatch);
			bool isValid();
			void invalidate();
			void deleteData();
		};

		//////////////////////////////////////////////////////////////////////////
		// cgl input factory data provider
		__interface CGL_API ICGLInputProcessorDataProvider
		{
			bool getElement(char* pDest, UINT elementIndex, cgl::core::CGL_INPUT_ELEMENT_SIGNATURE* inputElementSignature, UINT inputElementCount  );
			bool isCompatible(std::vector<cgl::core::CGL_INPUT_ELEMENT_SIGNATURE>& pRequiredElements);	
		};

		//////////////////////////////////////////////////////////////////////////
		// cgl input data process mode
		enum CGL_PROCESS_MODE
		{
			CGL_PROCESS_MODE_PER_ELEMENT,
			CGL_PROCESS_MODE_PER_SUB_ELEMENT,
		};

		//////////////////////////////////////////////////////////////////////////
		// cgl input data processor
		class CGL_API CGLInputDataProcessor : public cgl::util::ICGLInputBufferDataProvider	
		{	
		private:
			CGL_PROCESS_MODE m_mode;

			cgl::core::PD3D11InputLayout m_pInputLayout;
			cgl::util::ICGLInputProcessorDataProvider* m_pDataProvider;

			char* m_pData;
			UINT m_dataSize;
			UINT m_elementSize;
			UINT m_elementCount;
			UINT m_step;
			std::vector<cgl::core::CGL_INPUT_ELEMENT_SIGNATURE> m_currElements;
			bool m_isValid;

			char* Process();	

			CGLInputDataProcessor(cgl::core::PD3D11InputLayout pInputLayout, cgl::util::ICGLInputProcessorDataProvider* pDataProvider, UINT elementCount, UINT step = 1, CGL_PROCESS_MODE mode = CGL_PROCESS_MODE_PER_SUB_ELEMENT);
			CGLInputDataProcessor operator =(const CGLInputDataProcessor &);

		public:	
			static PCGLInputDataProcessor Create(cgl::core::PD3D11InputLayout pInputLayout, cgl::util::ICGLInputProcessorDataProvider* pDataProvider, UINT elementCount, UINT step = 1, CGL_PROCESS_MODE mode = CGL_PROCESS_MODE_PER_SUB_ELEMENT);
			~CGLInputDataProcessor();

			inline UINT getElementCount()	{ return m_dataSize / m_elementSize; }
			inline UINT getDataSize()		{ return m_dataSize; }
			inline UINT getElementSize()	{ return m_elementSize; }
			inline char* getData()			{ return Process(); }
			inline bool isValid()			{ return m_isValid; }
			inline void invalidate()		{ m_isValid = false; }
			inline void deleteData()		{ SAFE_FREE(m_pData); }

			bool isCompatible(cgl::core::PD3D11InputLayout& pLayoutToMatch);
		};
	}
}

#endif // CGLInputProcessor_h__