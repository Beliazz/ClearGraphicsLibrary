#include "cgl.h"

using namespace cgl;

template <class T>
cgl::CGLTypedBindable<T>::CGLTypedBindable( CGLBase<T>* pObject, UINT paramCount ) : m_pObject__(pObject)
{
	pObject->addFeatureFlag(CGL_OBJECT_FEATURES::CGL_OBJECT_BIND_FEATURE);

	if (paramCount > 0)
	{
		m_params.resize(paramCount);
		for (auto it = m_params.begin(); it != m_params.end(); it++)
		{
			(*it) = NULL;
		}
	}
}

template <class T>
cgl::CGLTypedBindable<T>::~CGLTypedBindable()
{
	m_pObject__->delFeatureFlag(CGL_OBJECT_FEATURES::CGL_OBJECT_BIND_FEATURE);
	for (auto it = m_params.begin(); it != m_params.end(); it++)
	{
		if (*it)
		{
			free(*it);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// cgl vertex buffer bindable
// 
const int CGLVertexBufferBindable::BindParamOffset = 0;
const int CGLVertexBufferBindable::BindParamSlot = 1;
const int CGLVertexBufferBindable::BindParamStride = 2;

void cgl::CGLVertexBufferBindable::bind()
{
	UINT slot = GetParam<UINT>(BindParamSlot);
	UINT stride = GetParam<UINT>(BindParamStride);
	UINT offset = GetParam<UINT>(BindParamOffset);
	m_pObject__->getDevice()->GetContext()->IASetVertexBuffers(GetParam<UINT>(BindParamSlot), 1, m_pObject__->ptr(), &GetParam<UINT>(BindParamStride), &GetParam<UINT>(BindParamOffset));
}

//////////////////////////////////////////////////////////////////////////
// cgl index buffer bindable
// 
const int CGLIndexBufferBindable::BindParamOffset = 0;
const int CGLIndexBufferBindable::BindParamFormat = 1; 

void cgl::CGLIndexBufferBindable::bind()
{
	m_pObject__->getDevice()->GetContext()->IASetIndexBuffer(m_pObject__->get(), GetParam<DXGI_FORMAT>(BindParamFormat),
																				 GetParam<int>(BindParamOffset));
}

//////////////////////////////////////////////////////////////////////////
// cgl rasterizer state bindable
// 
void cgl::CGLRasterizerStateBindable::bind()
{
	m_pObject__->getDevice()->GetContext()->RSSetState(m_pObject__->get());
}