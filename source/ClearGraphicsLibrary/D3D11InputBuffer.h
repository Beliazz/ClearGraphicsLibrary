#ifndef InputBuffer_h__
#define InputBuffer_h__

#include "cgl.h"

namespace cgl
{

//////////////////////////////////////////////////////////////////////////
// typedef
class CD3D11InputBuffer;
typedef CGL_API std::tr1::shared_ptr<CD3D11InputBuffer>	PD3D11InputBuffer;

//////////////////////////////////////////////////////////////////////////
// d3d11 input buffer
class CGL_API CD3D11InputBuffer : public CGLBase<ID3D11Buffer>, public CGLVertexBufferBindable
{	
private:
	D3D11_BUFFER_DESC m_desc;
	PD3D11InputLayout m_pLayout;
	ICGLInputBufferDataProvider* m_pDataProvider;

protected:
	HRESULT onRestore();
	void onReset();
	void getDependencies(std::vector<PCGLObject>* pDependencies);
	CD3D11InputBuffer(PD3D11InputLayout pLayout, ICGLInputBufferDataProvider* pDataProvider, D3D11_USAGE usage, DWORD cpuAccessFlags);
	
public:
	static PD3D11InputBuffer Create(PD3D11InputLayout pLayout, ICGLInputBufferDataProvider* pDataProvider, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, DWORD cpuAccessFlags = 0);
	static void Bind(PD3D11InputBuffer* pBuffers, UINT count, UINT slot = 0, UINT offset = 0);

	bool Update();
	void Draw();
	void Draw(UINT offset);
	void Draw(UINT offset, UINT count);

	inline UINT GetStride() { return m_pLayout->GetElementSize(); }
	inline bool IsValid()	{ return m_pDataProvider->isValid(); }
};

}

#endif // InputBuffer_h__

