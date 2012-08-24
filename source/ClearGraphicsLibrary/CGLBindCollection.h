#ifndef CGLBindCollection_h__
#define CGLBindCollection_h__

#include "cgl.h"

namespace cgl {

// typedef
class CGLShaderResourceViewCollection;
class CGLRenderTargetViewCollection;
typedef CGL_API std::tr1::shared_ptr<CGLShaderResourceViewCollection> PCGLShaderResourceViewCollection; 
typedef CGL_API std::tr1::shared_ptr<CGLRenderTargetViewCollection> PCGLRenderTargetViewCollection; 

//////////////////////////////////////////////////////////////////////////
// BindCollection
template <class T>
class CGL_API CGLBindCollection : public CGLBase<T*>
{
private:
	std::vector<std::tr1::shared_ptr<CGLBase<T>>> m_collection;

protected:
	virtual HRESULT onRestore();
	virtual void onReset();

	virtual void OnAdd() { };
	virtual void OnRemove() { };
public:
	CGLBindCollection(std::string className) 		: CGLBase(className)
	{
		//////////////////////////////////////////////////////////////////////////
		// template instantiation
		// 
		// this sick shit must be done otherwise
		// you get nasty linker errors
		// 
		// because of this explicit call
		// the compiler creates the template member functions
		// for the specific type
		Add(0, 0);
		Remove(0);
	}
	virtual ~CGLBindCollection() { onReset(); }
	virtual bool Update();

	inline void Add(std::tr1::shared_ptr<CGLBase<T>> object, UINT slot)
	{
		// resize vector
		while(slot >= m_collection.size()) {
			m_collection.push_back(NULL);
		}

		// set
		m_collection[slot] = object;

		OnAdd();
	}
	inline void Remove(UINT slot)
	{
		if (slot > 0 && slot < m_collection.size())
		{
			m_collection[slot] = NULL;
		}

		OnRemove();
	}
	inline std::vector<std::tr1::shared_ptr<CGLBase<T>>>& GetCollection() {return m_collection;}
	inline size_t GetCollectionSize() {return m_collection.size();}
};

//////////////////////////////////////////////////////////////////////////
// CGLSRVCollection
class CGL_API CGLShaderResourceViewCollection : public CGLBindCollection<ID3D11ShaderResourceView>
{
protected:
	void getDependencies(std::vector<PCGLObject>* pDependencies ) { }
	CGLShaderResourceViewCollection();

public:
	static PCGLShaderResourceViewCollection Create();

	// set all views starting at slot 0
	void Bind(CGL_SHADER_STAGE stage);

	// set all views starting at the specified slot
	void Bind(CGL_SHADER_STAGE stage, UINT startSlot);

	// set a custom number of views
	// if the number is greater than the current collection size
	// it numViews will be changed to the collection size
	void Bind(CGL_SHADER_STAGE stage, UINT startSlot, UINT numViews);
};

//////////////////////////////////////////////////////////////////////////
// CGLRTVCollection
class CGL_API CGLRenderTargetViewCollection : public CGLBindCollection<ID3D11RenderTargetView>
{
private:
	PD3D11DepthStencilView m_pDepthStencilView;
	ID3D11RenderTargetView** m_ppSavedCollection;
	ID3D11DepthStencilView* m_pSavedDepthStencilView;

protected:
	void getDependencies(std::vector<PCGLObject>* pDependencies ) { }
	CGLRenderTargetViewCollection();

public:
	~CGLRenderTargetViewCollection();
	static PCGLRenderTargetViewCollection Create();

	// set depth stencil view
	void SetDepthStencilView(PD3D11DepthStencilView pDepthStencilView);

	// set all views
	void Bind();

	// set a custom number of views
	// if the number is greater than the current collection size
	// it numViews will be changed to the collection size
	void Bind(UINT numViews);

	void Save();
	void Load();
};

//////////////////////////////////////////////////////////////////////////
// vertex buffer collection
class CGL_API CD3D11VertexBufferCollection : public CGLBindCollection<ID3D11Buffer>
{
private:
	UINT* m_pStrides;

protected:
	void OnAdd();
	void OnRemove();

public:
	~CD3D11VertexBufferCollection();

	// set all views
	void Bind();

	// set a custom number of views
	// if the number is greater than the current collection size
	// it numViews will be changed to the collection size
	void Bind(UINT numViews);
	void Bind(UINT numViews, UINT* pOffsets);
	void Bind(UINT startSlot, UINT numViews, UINT* pOffsets );
};

}

#endif // CGLBindCollection_h__