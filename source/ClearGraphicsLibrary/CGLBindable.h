#pragma once

#include "cgl.h"

namespace cgl
{

//////////////////////////////////////////////////////////////////////////
// bind object types
enum CGL_BIND
{
	CGL_BIND_VERTEXBUFFER,
	CGL_BIND_INDEXBUFFER,
	CGL_BIND_RASTERIZER_STATE,
	CGL_BIND_DEPTH_STENCIL_STATE,
	CGL_BIND_RENDERTARGET,
	CGL_BIND_BLEND_STATE,
	CGL_BIND_INPUT_LAYOUT,

	CGL_BIND_UNUSED
};

//////////////////////////////////////////////////////////////////////////
// bindable interface (untyped base class)
class CGL_API CGLBindable
{
friend class D3D11Device;
friend class CGLManager;

private:
	bool m_upToDate__;

protected:
	virtual void bind()	PURE;
	virtual CGL_BIND getBind() PURE;
	virtual UINT getBindId() PURE;

	inline bool valid()		 { return m_upToDate__; }
	inline void invalidate() { m_upToDate__ = false; }
	inline void validate()	 { m_upToDate__ = true;  }

public:
	CGLBindable() : m_upToDate__(false) { }
};

//////////////////////////////////////////////////////////////////////////
// typed base class
template <class T>
class CGL_API CGLTypedBindable : public CGLBindable
{
protected:
	CGLBase<T>* m_pObject__;
	std::vector<void*> m_params;

	inline UINT getBindId()  { return m_pObject__->getLuid(); }

public:
	CGLTypedBindable(CGLBase<T>* pObject, UINT paramCount);
	virtual ~CGLTypedBindable();
	void Bind() { m_pObject__->getDevice()->Bind(this); }

	template <class T>
	void SetParam(int param, T val)
	{
		if (!m_params[param])
		{
			m_params[param] = malloc(sizeof(T));
		}
		
		memcpy(m_params[param], &val, sizeof(T));
	}

	template <class T>
	inline T& GetParam(int param) { return *reinterpret_cast<T*>(m_params[param]); }
};

//////////////////////////////////////////////////////////////////////////
// bind implementation for vertex buffers
class CGL_API CGLVertexBufferBindable : public CGLTypedBindable<ID3D11Buffer>
{
protected:
	void bind();
	inline CGL_BIND getBind() { return CGL_BIND_VERTEXBUFFER; }

public:
	CGLVertexBufferBindable(CGLBase<ID3D11Buffer>* pVertexBuffer) 
		: CGLTypedBindable(pVertexBuffer, 3) { }

	static const int BindParamSlot;
	static const int BindParamOffset;
	static const int BindParamStride;
};

//////////////////////////////////////////////////////////////////////////
// bind implementation for index buffers
class CGL_API CGLIndexBufferBindable : public CGLTypedBindable<ID3D11Buffer>
{
protected:
	void bind();
	inline CGL_BIND getBind() { return CGL_BIND_INDEXBUFFER; }

public:
	CGLIndexBufferBindable(CGLBase<ID3D11Buffer>* pIndexBuffer) 
		: CGLTypedBindable(pIndexBuffer, 2) { }

	static const int BindParamOffset;
	static const int BindParamFormat;
};

//////////////////////////////////////////////////////////////////////////
// bind implementation for rasterizer states
class CGL_API CGLRasterizerStateBindable : public CGLTypedBindable<ID3D11RasterizerState>
{
private:

protected:
	void bind();
	inline CGL_BIND getBind() { return CGL_BIND_RASTERIZER_STATE; }

public:
	CGLRasterizerStateBindable(CGLBase<ID3D11RasterizerState>* pRasterizerState) 
		: CGLTypedBindable(pRasterizerState, 0) { }
};

}