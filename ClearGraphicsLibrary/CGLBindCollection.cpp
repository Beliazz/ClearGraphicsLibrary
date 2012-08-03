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
cgl::CGLRenderTargetViewCollection::CGLRenderTargetViewCollection() : CGLBindCollection("CD3D11RTVCollection") { }
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
