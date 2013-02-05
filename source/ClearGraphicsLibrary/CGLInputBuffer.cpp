#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// d3d11 input buffer
cgl::util::CGLInputBuffer::CGLInputBuffer(cgl::core::PD3D11InputLayout pLayout, ICGLInputBufferDataProvider* pDataProvider, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, DWORD cpuAccessFlags /*= 0*/) 
	: CGLBase("CD3D11InputBuffer"), CGLTypedBindable(this), m_pLayout(pLayout), m_pDataProvider(pDataProvider), m_desc(CD3D11_BUFFER_DESC(0, D3D11_BIND_VERTEX_BUFFER, usage, cpuAccessFlags))
{
	SetParam<UINT>(BIND_PARAM_STRIDE_UINT, 1);
	SetParam<UINT>(BIND_PARAM_OFFSET_UINT, 0);
	SetParam<UINT>(BIND_PARAM_SLOT_UINT, 0);
}
HRESULT cgl::util::CGLInputBuffer::onRestore()
{
	if (m_pDataProvider->isValid())
	{
		if (m_pDataProvider->isCompatible(m_pLayout))
		{	
			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = m_pDataProvider->getData();		
			m_desc.ByteWidth = m_pDataProvider->getDataSize();
			m_desc.StructureByteStride = m_pDataProvider->getElementSize();

			onParamUpdate();

			if (get())
			{
				switch (m_desc.Usage)
				{
				//////////////////////////////////////////////////////////////////////////
				// default
				case D3D11_USAGE_DEFAULT:
					{
						cgl::core::CGLAccess::D3DContext()->UpdateSubresource(get(), 0, NULL, data.pSysMem, 0, 0);
						return S_OK;
					} break;
				//////////////////////////////////////////////////////////////////////////
				// dynamic
				case D3D11_USAGE_DYNAMIC:
					{
						D3D11_MAPPED_SUBRESOURCE subresource;
						HRESULT result = cgl::core::CGLAccess::D3DContext()->Map(get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &subresource );
						if (SUCCEEDED(result))
						{
							memcpy(subresource.pData, data.pSysMem, m_desc.ByteWidth);
							cgl::core::CGLAccess::D3DContext()->Unmap(get(), 0);
							return S_OK;
						}

						return E_FAIL;

					} break;

				//////////////////////////////////////////////////////////////////////////
				// immutable, staging
				default:
					reset();
				}
			}

			HRESULT result =  cgl::core::CGLAccess::D3DDevice()->CreateBuffer(&m_desc, &data, ptr());
			m_pDataProvider->deleteData();
			return result;
		}
		else
		{
			m_pDataProvider->invalidate();
			return E_FAIL;
		}
	}

	return E_FAIL;
}
void cgl::util::CGLInputBuffer::onReset()
{
	comReset((IUnknown**)ptr());
}
void cgl::util::CGLInputBuffer::getDependencies( std::vector<cgl::core::PCGLObject>* pDependencies )
{
	pDependencies->push_back(m_pLayout);
}
cgl::util::PCGLInputBuffer cgl::util::CGLInputBuffer::Create(cgl::core::PD3D11InputLayout pLayout, ICGLInputBufferDataProvider* pDataProvider, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, DWORD cpuAccessFlags /*= 0*/)
{
	return create<CGLInputBuffer>(new CGLInputBuffer(pLayout, pDataProvider, usage, cpuAccessFlags));
}

void cgl::util::CGLInputBuffer::Draw( UINT offset, UINT count)
{
	Bind();
	cgl::core::CGLAccess::D3DContext()->Draw(count, offset);
}
void cgl::util::CGLInputBuffer::Draw()
{
	Bind();
	Draw(0, m_pDataProvider->getElementCount());
}
void cgl::util::CGLInputBuffer::Draw( UINT offset )
{
	Bind();
	Draw(offset, m_pDataProvider->getElementCount());
}
bool cgl::util::CGLInputBuffer::Update()
{
	return restore();
}
void cgl::util::CGLInputBuffer::onParamUpdate()
{
	SetParam<UINT>(BIND_PARAM_STRIDE_UINT, GetStride());
}

void cgl::util::CGLInputBuffer::bind()
{
	cgl::core::CGLAccess::D3DContext()->IASetVertexBuffers(GetParam<UINT>(BIND_PARAM_SLOT_UINT), 
														   1,
														   ptr(),
														   &GetParam<UINT>(BIND_PARAM_STRIDE_UINT),
														   &GetParam<UINT>(BIND_PARAM_OFFSET_UINT));
}

