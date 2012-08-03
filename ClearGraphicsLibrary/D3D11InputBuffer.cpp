#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// d3d11 input buffer
cgl::CD3D11InputBuffer::CD3D11InputBuffer(PD3D11InputLayout pLayout, ICGLInputBufferDataProvider* pDataProvider, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, DWORD cpuAccessFlags /*= 0*/) 
	: CGLBase("CD3D11InputBuffer"), m_pLayout(pLayout), m_pDataProvider(pDataProvider), m_desc(CD3D11_BUFFER_DESC(0, D3D11_BIND_VERTEX_BUFFER, usage, cpuAccessFlags))
{
}
HRESULT cgl::CD3D11InputBuffer::onRestore()
{
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));

	if (m_pDataProvider->isValid())
	{
		if (m_pDataProvider->isCompatible(m_pLayout))
		{
			data.pSysMem = m_pDataProvider->getData();
			m_desc.ByteWidth = m_pDataProvider->getDataSize();

			HRESULT result =  getDevice()->GetDevice()->CreateBuffer(&m_desc, &data, ptr());

			m_pDataProvider->deleteData();

			return result;
		}
		else
		{
			m_pDataProvider->invalidate();
		}
	}

	return getDevice()->GetDevice()->CreateBuffer(&m_desc, NULL, ptr());
}
void cgl::CD3D11InputBuffer::onReset()
{
	comReset((IUnknown**)ptr());
}
void cgl::CD3D11InputBuffer::getDependencies( std::vector<PCGLObject>* pDependencies )
{
	pDependencies->push_back(m_pLayout);
}
cgl::PD3D11InputBuffer cgl::CD3D11InputBuffer::Create(PD3D11InputLayout pLayout, ICGLInputBufferDataProvider* pDataProvider, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, DWORD cpuAccessFlags /*= 0*/)
{
	return create<CD3D11InputBuffer>(new CD3D11InputBuffer(pLayout, pDataProvider, usage, cpuAccessFlags));
}
void cgl::CD3D11InputBuffer::Bind(UINT slot, UINT offset)
{
	UINT stride = m_pLayout->GetElementSize();
	getDevice()->GetContext()->IASetVertexBuffers(slot, 1, ptr(), &stride, &offset );
}
void cgl::CD3D11InputBuffer::Bind( PD3D11InputBuffer* pBuffers, UINT count, UINT slot /*= 0*/, UINT offset /*= 0*/ )
{
	UINT* pOffsets = new UINT[count];
	UINT* pStrides = new UINT[count];
	ID3D11Buffer** ppBuffers = new ID3D11Buffer*[count];

	for (UINT i = 0; i < count; i++)
	{
		pStrides[i] = pBuffers[i]->GetStride();
		ppBuffers[i] = pBuffers[i]->get();
	}

	CGLManager::GetInstance()->GetDevice()->GetContext()->IASetVertexBuffers(slot, count, ppBuffers, pStrides, pOffsets);

	SAFE_DELETE_ARRAY(pOffsets);
	SAFE_DELETE_ARRAY(pStrides);
	SAFE_DELETE_ARRAY(ppBuffers);
}
void cgl::CD3D11InputBuffer::Draw( UINT offset, UINT count)
{
	getDevice()->GetContext()->Draw(count, offset);
}
void cgl::CD3D11InputBuffer::Draw()
{
	Draw(0, m_pDataProvider->getElementCount());
}
void cgl::CD3D11InputBuffer::Draw( UINT offset )
{
	Draw(offset, m_pDataProvider->getElementCount());
}
bool cgl::CD3D11InputBuffer::Update()
{
	if (!get())
	{
		reset();
		return restore();
	}

	// get buffer size
	D3D11_BUFFER_DESC desc;
	get()->GetDesc(&desc);
	if (m_pDataProvider->getDataSize() != desc.ByteWidth)
	{
		reset();
		return restore();
	}

	switch(m_desc.Usage)
	{
	case D3D11_USAGE_DEFAULT:
		{
			getDevice()->GetContext()->UpdateSubresource(get(), 0, NULL, m_pDataProvider->getData(), 0, 0);
			m_pDataProvider->deleteData();
			return true;
		} break;

	case D3D11_USAGE_IMMUTABLE:
	case D3D11_USAGE_STAGING:	
		{
			return restore();
		} break;

	case D3D11_USAGE_DYNAMIC:
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			HRESULT result = getDevice()->GetContext()->Map(get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &subresource );
			if (SUCCEEDED(result))
			{
				memcpy(subresource.pData, m_pDataProvider->getData(),  m_pDataProvider->getDataSize());
				getDevice()->GetContext()->Unmap(get(), 0);
				m_pDataProvider->deleteData();
			}

			return true;
		} break;		
	}

	return false;
}