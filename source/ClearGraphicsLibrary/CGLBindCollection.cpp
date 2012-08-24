#include "cgl.h"

template <class T>
bool cgl::CGLBindCollection<T>::Update()
{
	return restore();
}

template <class T>
HRESULT cgl::CGLBindCollection<T>::onRestore()
{
	// reset ptr
	onReset();

	// get collection from parent class
	std::vector<std::tr1::shared_ptr<CGLBase<T>>>& collection = GetCollection();

	// allocate memory for the ptr arr
	set((T**)malloc( sizeof(T*) * collection.size()));

	// set all ptr NULL
	ZeroMemory(get(), sizeof(T*) * collection.size());

	// extract ptr from the collection
	for (UINT i = 0; i < collection.size(); i++)
	{
		if (collection[i])
			((T**)get())[i] = collection[i]->get();
	}

	return S_OK;
}

template <class T>
void cgl::CGLBindCollection<T>::onReset()
{
	if (get())
	{
		free(get());
		set(NULL);
	}
}


//////////////////////////////////////////////////////////////////////////
// cgl shader resource view collection
cgl::CGLShaderResourceViewCollection::CGLShaderResourceViewCollection() : CGLBindCollection("CD3D11SRVCollection") {}
void cgl::CGLShaderResourceViewCollection::Bind( CGL_SHADER_STAGE stage )
{
	Bind(stage, 0, GetCollectionSize());
}
void cgl::CGLShaderResourceViewCollection::Bind( CGL_SHADER_STAGE stage, UINT startSlot )
{
	Bind(stage, startSlot, GetCollectionSize());
}
void cgl::CGLShaderResourceViewCollection::Bind( CGL_SHADER_STAGE stage, UINT startSlot, UINT numViews)
{
	if (startSlot < 0 || startSlot >= GetCollectionSize())
		return;

	if (numViews > GetCollectionSize() )
	{
		numViews = GetCollectionSize();
	}

	switch(stage)
	{
	case CGL_SHADER_STAGE_VERTEX: 	getDevice()->GetContext()->VSSetShaderResources(startSlot, numViews, get()); break;
	case CGL_SHADER_STAGE_PIXEL: 	getDevice()->GetContext()->PSSetShaderResources(startSlot, numViews, get()); break;
	case CGL_SHADER_STAGE_GEOMETRY: getDevice()->GetContext()->GSSetShaderResources(startSlot, numViews, get()); break;
	case CGL_SHADER_STAGE_HULL: 	getDevice()->GetContext()->HSSetShaderResources(startSlot, numViews, get()); break;
	case CGL_SHADER_STAGE_DOMAIN: 	getDevice()->GetContext()->DSSetShaderResources(startSlot, numViews, get()); break;
	}
}
cgl::PCGLShaderResourceViewCollection cgl::CGLShaderResourceViewCollection::Create()
{
	return create<cgl::CGLShaderResourceViewCollection>(new CGLShaderResourceViewCollection());
}

//////////////////////////////////////////////////////////////////////////
// cgl render target view collection
cgl::CGLRenderTargetViewCollection::CGLRenderTargetViewCollection()
	: CGLBindCollection("CD3D11RTVCollection"), m_ppSavedCollection(NULL), m_pSavedDepthStencilView(NULL) { }
cgl::CGLRenderTargetViewCollection::~CGLRenderTargetViewCollection()
{
	if (m_ppSavedCollection)
	{
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			SAFE_RELEASE(m_ppSavedCollection[i]);

		SAFE_DELETE(m_ppSavedCollection);
		SAFE_RELEASE(m_pSavedDepthStencilView);
	}
}
void cgl::CGLRenderTargetViewCollection::Bind( )
{
	Bind(GetCollectionSize());
}
void cgl::CGLRenderTargetViewCollection::Bind( UINT numViews )
{
	if (numViews > GetCollectionSize() )
	{
		numViews = GetCollectionSize();
	}

	if (m_pDepthStencilView)
	{
		getDevice()->GetContext()->OMSetRenderTargets(numViews, get(), m_pDepthStencilView->get());
	}
	else
	{
		getDevice()->GetContext()->OMSetRenderTargets(numViews, get(), NULL);
	}

}
void cgl::CGLRenderTargetViewCollection::SetDepthStencilView( PD3D11DepthStencilView pDepthStencilView )
{
	m_pDepthStencilView = pDepthStencilView;
}
cgl::PCGLRenderTargetViewCollection cgl::CGLRenderTargetViewCollection::Create()
{
	return create<CGLRenderTargetViewCollection>(new CGLRenderTargetViewCollection());
}
void cgl::CGLRenderTargetViewCollection::Save()
{
	if (m_ppSavedCollection)
	{
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			SAFE_RELEASE(m_ppSavedCollection[i]);

		SAFE_DELETE(m_ppSavedCollection);
		SAFE_RELEASE(m_pSavedDepthStencilView);
	}

	m_ppSavedCollection = new ID3D11RenderTargetView*[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	mgr()->GetDevice()->GetContext()->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, m_ppSavedCollection, &m_pSavedDepthStencilView );
}
void cgl::CGLRenderTargetViewCollection::Load()
{
	if (m_ppSavedCollection)
	{
		mgr()->GetDevice()->GetContext()->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, m_ppSavedCollection, m_pSavedDepthStencilView );
	}
}

//////////////////////////////////////////////////////////////////////////
// cgl vertex buffer collection
void cgl::CD3D11VertexBufferCollection::Bind()
{
	Bind(GetCollectionSize());
}
void cgl::CD3D11VertexBufferCollection::Bind(UINT numViews )
{
	UINT* pOffsets = new UINT[numViews];
	ZeroMemory(pOffsets, numViews * sizeof(UINT));
	Bind(numViews, pOffsets);
	SAFE_DELETE(pOffsets);
}
void cgl::CD3D11VertexBufferCollection::Bind(UINT numViews, UINT* pOffsets )
{
	Bind(0, numViews, pOffsets);
}
void cgl::CD3D11VertexBufferCollection::Bind(UINT startSlot, UINT numViews, UINT* pOffsets )
{
	getDevice()->GetContext()->IASetVertexBuffers(startSlot, numViews, get(), m_pStrides, pOffsets );
}

void cgl::CD3D11VertexBufferCollection::OnAdd()
{
	SAFE_DELETE(m_pStrides);

	std::vector<std::tr1::shared_ptr<cgl::CGLBase<ID3D11Buffer>>>& collection = GetCollection();
	for(UINT i = 0; i < GetCollectionSize(); i++)
	{
		D3D11_BUFFER_DESC desc;
		collection[i]->get()->GetDesc(&desc);
		m_pStrides[i] = desc.StructureByteStride;
	}
}
void cgl::CD3D11VertexBufferCollection::OnRemove()
{
	SAFE_DELETE(m_pStrides);

	std::vector<std::tr1::shared_ptr<cgl::CGLBase<ID3D11Buffer>>>& collection = GetCollection();
	for(UINT i = 0; i < GetCollectionSize(); i++)
	{
		D3D11_BUFFER_DESC desc;
		collection[i]->get()->GetDesc(&desc);
		m_pStrides[i] = desc.StructureByteStride;
	}
}

cgl::CD3D11VertexBufferCollection::~CD3D11VertexBufferCollection()
{
	SAFE_DELETE(m_pStrides);
}


