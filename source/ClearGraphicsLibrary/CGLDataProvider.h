#ifndef CGLDataProvider_h__
#define CGLDataProvider_h__

#include "cgl.h"

namespace cgl
{
		
//////////////////////////////////////////////////////////////////////////
// typedef
struct CGL_INPUT_ELEMENT_SIGNATURE;
class CD3D11InputLayout;
typedef CGL_API std::tr1::shared_ptr<CD3D11InputLayout> PD3D11InputLayout;

//////////////////////////////////////////////////////////////////////////
// cgl input buffer data provider
__interface CGL_API ICGLInputBufferDataProvider	
{
	UINT getElementCount();
	UINT getDataSize();
	UINT getElementSize();
	char* getData();
	bool isCompatible(PD3D11InputLayout& pLayoutToMatch);
	bool isValid();
	void invalidate();
	void deleteData();
};

//////////////////////////////////////////////////////////////////////////
// cgl input factory data provider
__interface CGL_API ICGLInputProcessorDataProvider
{
	bool getElement(char* pDest, UINT elementIndex, CGL_INPUT_ELEMENT_SIGNATURE* inputElementSignature, UINT inputElementCount  );
	bool isCompatible(std::vector<CGL_INPUT_ELEMENT_SIGNATURE>& pRequiredElements);	
};

}

#endif // CGLDataProvider_h__

