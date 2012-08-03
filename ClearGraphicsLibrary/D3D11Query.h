#ifndef D3D11Query_h__
#define D3D11Query_h__

#include "cgl.h"

namespace cgl {


//////////////////////////////////////////////////////////////////////////
// d3d11 query
class CGL_API CD3D11Query : public CGLBase<ID3D11Query>
{
private:
	D3D11_QUERY_DESC m_desc;

protected:
	CD3D11Query(D3D11_QUERY_DESC desc);
	HRESULT	onRestore();
	void onReset();
	void getDependencies(std::vector<PCGLObject>* pDependencies ) { }

public:
	static std::tr1::shared_ptr<CD3D11Query> Create(D3D11_QUERY_DESC desc);

	void Begin();
	void End();
	void GetData(void* pData);
	inline size_t GetDataSize() { return get()->GetDataSize(); } 
};

//////////////////////////////////////////////////////////////////////////
// typedefs
typedef CGL_API std::tr1::shared_ptr<CD3D11Query> PD3D11Query;

}
#endif // D3D11Query_h__
