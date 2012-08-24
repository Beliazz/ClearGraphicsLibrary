#ifndef D3D11InputLayout_h__
#define D3D11InputLayout_h__

#include "cgl.h"

namespace cgl
{

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

//////////////////////////////////////////////////////////////////////////
// d3d11 input layout
class CGL_API CD3D11InputLayout : public CGLBase<ID3D11InputLayout>
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

public:
	~CD3D11InputLayout();
	static PD3D11InputLayout Create( PD3D11EffectPass pPass, ICGLInstancing* pInstancingInfo = NULL);
	
	inline ICGLInstancing* GetInstancingInfo()							{ return m_pInstancingInfo; }
	inline std::vector<CGL_INPUT_ELEMENT_SIGNATURE>& GetInputElements()	{ return m_cglElements; }
	inline UINT GetElementSize()										{ return m_vertexSize; }

	void Bind();

	bool operator == (CD3D11InputLayout& rhs);
};

}

#endif // D3D11InputLayout_h__