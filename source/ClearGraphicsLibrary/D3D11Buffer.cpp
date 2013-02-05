#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// d3d11 buffer
cgl::core::CD3D11Buffer::CD3D11Buffer(UINT stride, D3D11_BUFFER_DESC desc)
	: CGLBase("CD3D11Buffer"), m_desc(desc), m_stride(stride)
{

}
cgl::core::CD3D11Buffer::~CD3D11Buffer()
{
	m_data.clear();
}
HRESULT cgl::core::CD3D11Buffer::onRestore()
{
	if (GetDataSize() == 0)
		return false;

	m_desc.ByteWidth = m_data.size();
	m_desc.StructureByteStride = m_stride;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = m_data.data();

	if (get())
	{
		switch (m_desc.Usage)
		{
		case D3D11_USAGE_DEFAULT:
			{
				CGLAccess::D3DContext()->UpdateSubresource(get(), 0, NULL, m_data.data(), 0, 0);

				// bind param update
				onParamUpdate();

				return S_OK;

			} break;

		case D3D11_USAGE_DYNAMIC:
			{
				D3D11_MAPPED_SUBRESOURCE subresource;
				HRESULT result = CGLAccess::D3DContext()->Map(get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &subresource );
				if (SUCCEEDED(result))
				{
					memcpy(subresource.pData, m_data.data(), m_data.size());
					CGLAccess::D3DContext()->Unmap(get(), 0);

					return S_OK;
				}

				// bind param update
				onParamUpdate();

				return E_FAIL;

			} break;

		default:
			reset();
		}
	}

	HRESULT hResult =  CGLAccess::D3DDevice()->CreateBuffer(&m_desc, &initData, ptr());

	// bind param update
	onParamUpdate();

	return hResult;
}
void cgl::core::CD3D11Buffer::onReset()
{
	comReset((IUnknown**)ptr());
}
size_t cgl::core::CD3D11Buffer::GetDataSize()
{
	return m_data.size();
}
size_t cgl::core::CD3D11Buffer::GetBufferSize()
{
	return m_desc.ByteWidth;
}
cgl::core::PD3D11Buffer cgl::core::CD3D11Buffer::Create( UINT elementSize, D3D11_BUFFER_DESC desc )
{
	return create(new CD3D11Buffer(elementSize, desc));
}

UINT cgl::core::CD3D11Buffer::SetData( char* pData, size_t count )
{
	// delete old data
	ResetData();

	// resize 
	m_data.resize(count * m_stride);

	// copy data
	memcpy(m_data.data(), pData, count * m_stride);

	// base index
	return 0;
}
UINT cgl::core::CD3D11Buffer::AddData( char* pData )
{
	UINT start = m_data.size();
	m_data.resize(m_data.size() + m_stride);
	memcpy(&m_data.data()[start], pData, m_stride);

	// base index
	return start;
}
UINT cgl::core::CD3D11Buffer::AddData( char* pData, size_t count )
{
	// copy data
	UINT start = m_data.size();
	UINT cursor = 0;
	m_data.resize(m_data.size() + count * m_stride);
	memcpy(&m_data.data()[start], pData, m_stride * count);

	return start;
}
void cgl::core::CD3D11Buffer::ResetData()
{
	m_data.clear();
}

//////////////////////////////////////////////////////////////////////////
// d3d11 vertex buffer
cgl::core::CD3D11VertexBuffer::CD3D11VertexBuffer(UINT stride, D3D11_USAGE usage, UINT cpuAccessFlags, UINT miscFlags ) 
	: CD3D11Buffer(stride, CD3D11_BUFFER_DESC(0, D3D11_BIND_VERTEX_BUFFER, usage, cpuAccessFlags, miscFlags )), CGLTypedBindable(this)
{
	SetParam<UINT>(BIND_PARAM_STRIDE_UINT, 1);
	SetParam<UINT>(BIND_PARAM_OFFSET_UINT, 0);
	SetParam<UINT>(BIND_PARAM_SLOT_UINT, 0);
}
cgl::core::PD3D11VertexBuffer cgl::core::CD3D11VertexBuffer::Create(UINT stride, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, UINT cpuAccessFlags /*= 0*/, UINT miscFlags /*= 0*/ )
{
	return create(new CD3D11VertexBuffer(stride, usage, cpuAccessFlags, miscFlags));
}
void cgl::core::CD3D11VertexBuffer::Draw( UINT offset, UINT count)
{
	CGLAccess::D3DContext()->Draw(count, offset);
}
void cgl::core::CD3D11VertexBuffer::Draw()
{
	Draw(0, m_desc.ByteWidth / m_stride);
}
void cgl::core::CD3D11VertexBuffer::Draw( UINT offset )
{
	Draw(offset, m_desc.ByteWidth / m_stride);
}
void cgl::core::CD3D11VertexBuffer::onParamUpdate()
{
	SetParam(BIND_PARAM_STRIDE_UINT, GetStride());
}

void cgl::core::CD3D11VertexBuffer::bind()
{
	CGLAccess::D3DContext()->IASetVertexBuffers(GetParam<UINT>(BIND_PARAM_SLOT_UINT), 
												1, 
												ptr(),
												&GetParam<UINT>(BIND_PARAM_STRIDE_UINT), 
												&GetParam<UINT>(BIND_PARAM_OFFSET_UINT) );
}


//////////////////////////////////////////////////////////////////////////
// d3d11 index buffer
cgl::core::CD3D11IndexBuffer::CD3D11IndexBuffer( UINT elementSize, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, UINT cpuAccessFlags /*= 0*/, UINT miscFlags /*= 0*/ )
	: CD3D11Buffer(elementSize, CD3D11_BUFFER_DESC(0, D3D11_BIND_INDEX_BUFFER, usage, cpuAccessFlags, miscFlags, elementSize )), CGLTypedBindable(this)
{
	SetParam(BIND_PARAM_FORMAT_DXGI_FORMAT, DXGI_FORMAT_R32_UINT);
	SetParam(BIND_PARAM_OFFSET_UINT, 0);
}
std::tr1::shared_ptr<cgl::core::CD3D11IndexBuffer> cgl::core::CD3D11IndexBuffer::Create( UINT elementSize, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, UINT cpuAccessFlags /*= 0*/, UINT miscFlags /*= 0*/ )
{
	return create(new CD3D11IndexBuffer(elementSize, usage, cpuAccessFlags, miscFlags));
}
void cgl::core::CD3D11IndexBuffer::Draw( UINT indexOffset, UINT vertexOffset)
{
	Draw(indexOffset, m_desc.ByteWidth / m_stride, vertexOffset);
}
void cgl::core::CD3D11IndexBuffer::Draw()
{
	Draw(0, m_desc.ByteWidth / m_stride, 0);
}
void cgl::core::CD3D11IndexBuffer::Draw( UINT indexOffset, UINT indexCount, UINT vertexOffset )
{
	Bind();
	CGLAccess::D3DContext()->DrawIndexed(indexCount, indexOffset, vertexOffset);
}
void cgl::core::CD3D11IndexBuffer::onParamUpdate()
{
	if (m_stride == 32)
	{
		SetParam(BIND_PARAM_FORMAT_DXGI_FORMAT, DXGI_FORMAT_R32_UINT);
	}
	else if (m_stride == 16)
	{
		SetParam(BIND_PARAM_FORMAT_DXGI_FORMAT, DXGI_FORMAT_R16_UINT);
	}
}

void cgl::core::CD3D11IndexBuffer::bind()
{
	CGLAccess::D3DContext()->IASetIndexBuffer(get(), 
											  GetParam<DXGI_FORMAT>(BIND_PARAM_FORMAT_DXGI_FORMAT), 
											  GetParam<int>(BIND_PARAM_OFFSET_UINT));
}

//
// cgl vertex buffer collection
void cgl::core::CD3D11VertexBufferCollection::Bind()
{
	Bind(GetCollectionSize());
}
void cgl::core::CD3D11VertexBufferCollection::Bind(UINT numViews )
{
	UINT* pOffsets = new UINT[numViews];
	ZeroMemory(pOffsets, numViews * sizeof(UINT));
	Bind(numViews, pOffsets);
	SAFE_DELETE(pOffsets);
}
void cgl::core::CD3D11VertexBufferCollection::Bind(UINT numViews, UINT* pOffsets )
{
	Bind(0, numViews, pOffsets);
}
void cgl::core::CD3D11VertexBufferCollection::Bind(UINT startSlot, UINT numViews, UINT* pOffsets )
{
	CGLAccess::D3DContext()->IASetVertexBuffers(startSlot, numViews, get(), m_pStrides, pOffsets );
}
void cgl::core::CD3D11VertexBufferCollection::OnAdd()
{
	SAFE_DELETE(m_pStrides);

	std::vector<std::tr1::shared_ptr<cgl::core::CGLBase<ID3D11Buffer>>>& collection = GetCollection();
	for(UINT i = 0; i < GetCollectionSize(); i++)
	{
		D3D11_BUFFER_DESC desc;
		collection[i]->get()->GetDesc(&desc);
		m_pStrides[i] = desc.StructureByteStride;
	}
}
void cgl::core::CD3D11VertexBufferCollection::OnRemove()
{
	SAFE_DELETE(m_pStrides);

	std::vector<std::tr1::shared_ptr<cgl::core::CGLBase<ID3D11Buffer>>>& collection = GetCollection();
	for(UINT i = 0; i < GetCollectionSize(); i++)
	{
		D3D11_BUFFER_DESC desc;
		collection[i]->get()->GetDesc(&desc);
		m_pStrides[i] = desc.StructureByteStride;
	}
}
cgl::core::CD3D11VertexBufferCollection::~CD3D11VertexBufferCollection()
{
	SAFE_DELETE(m_pStrides);
}

//////////////////////////////////////////////////////////////////////////
// d3d11 constant buffer
cgl::core::CD3D11ConstantBuffer::CD3D11ConstantBuffer( UINT elementSize, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, UINT cpuAccessFlags /*= 0*/, UINT miscFlags /*= 0*/ )
	: CD3D11Buffer(elementSize, CD3D11_BUFFER_DESC(0, D3D11_BIND_CONSTANT_BUFFER, usage, cpuAccessFlags, miscFlags, elementSize ))
{

}
void cgl::core::CD3D11ConstantBuffer::Bind(PD3D11EffectVariable pVar)
{
	pVar->get()->AsConstantBuffer()->SetConstantBuffer(get());
}
std::tr1::shared_ptr<cgl::core::CD3D11ConstantBuffer> cgl::core::CD3D11ConstantBuffer::Create( UINT elementSize, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, UINT cpuAccessFlags /*= 0*/, UINT miscFlags /*= 0*/ )
{
	return create<CD3D11ConstantBuffer>(new CD3D11ConstantBuffer(elementSize, usage, cpuAccessFlags, miscFlags));
}

