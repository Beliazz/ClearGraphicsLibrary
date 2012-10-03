#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// d3d11 buffer
cgl::CD3D11Buffer::CD3D11Buffer(UINT stride, D3D11_BUFFER_DESC desc)
	: CGLBase("CD3D11Buffer"), m_desc(desc), m_stride(stride)
{

}
cgl::CD3D11Buffer::~CD3D11Buffer()
{
	m_data.clear();
}
HRESULT cgl::CD3D11Buffer::onRestore()
{
	m_desc.ByteWidth = m_data.size();
	m_desc.StructureByteStride = m_stride;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = m_data.data();

	HRESULT result = getDevice()->GetDevice()->CreateBuffer(&m_desc, &initData, ptr());

	return result;
}
void cgl::CD3D11Buffer::onReset()
{
	comReset((IUnknown**)ptr());
}

bool cgl::CD3D11Buffer::Update()
{
	if (GetDataSize() == 0)
		return false;

	if (GetDataSize() != GetBufferSize())
	{
		reset();
		return restore();
	}

	switch(m_desc.Usage)
	{
	case D3D11_USAGE_DEFAULT:
		{
			getDevice()->GetContext()->UpdateSubresource(get(), 0, NULL, m_data.data(), 0, 0);
			return true;

		} break;

	case D3D11_USAGE_IMMUTABLE:
	case D3D11_USAGE_STAGING:	
		{
			reset();
			return restore();

		} break;

	case D3D11_USAGE_DYNAMIC:
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			HRESULT result = getDevice()->GetContext()->Map(get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &subresource );
			if (SUCCEEDED(result))
			{
				memcpy(subresource.pData, m_data.data(), m_data.size());
				getDevice()->GetContext()->Unmap(get(), 0);

				return true;
			}
			
			return false;

		} break;		
	}

	return false;
}
size_t cgl::CD3D11Buffer::GetDataSize()
{
	return m_data.size();
}
size_t cgl::CD3D11Buffer::GetBufferSize()
{
	if(get())
	{
		D3D11_BUFFER_DESC desc;
		get()->GetDesc(&desc);
		return desc.ByteWidth;
	}

	return 0;
}
cgl::PD3D11Buffer cgl::CD3D11Buffer::Create( UINT elementSize, D3D11_BUFFER_DESC desc )
{
	return create<CD3D11Buffer>(new CD3D11Buffer(elementSize, desc));
}

UINT cgl::CD3D11Buffer::SetData( char* pData, size_t count )
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
UINT  cgl::CD3D11Buffer::AddData( char* pData )
{
	UINT start = m_data.size();
	m_data.resize(m_data.size() + m_stride);
	memcpy(&m_data.data()[start], pData, m_stride);

	// base index
	return start;
}
UINT cgl::CD3D11Buffer::AddData( char* pData, size_t count )
{
	// copy data
	UINT start = m_data.size();
	UINT cursor = 0;
	m_data.resize(m_data.size() + count * m_stride);
	memcpy(&m_data.data()[start], pData, m_stride * count);

	return start;
}
void cgl::CD3D11Buffer::ResetData()
{
	m_data.clear();
}

//////////////////////////////////////////////////////////////////////////
// d3d11 vertex buffer
cgl::CD3D11VertexBuffer::CD3D11VertexBuffer(UINT stride, D3D11_USAGE usage, UINT cpuAccessFlags, UINT miscFlags ) 
	: CD3D11Buffer(stride, CD3D11_BUFFER_DESC(0, D3D11_BIND_VERTEX_BUFFER, usage, cpuAccessFlags, miscFlags ))
{

}
cgl::PD3D11VertexBuffer cgl::CD3D11VertexBuffer::Create(UINT stride, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, UINT cpuAccessFlags /*= 0*/, UINT miscFlags /*= 0*/ )
{
	return create<CD3D11VertexBuffer>(new CD3D11VertexBuffer(stride, usage, cpuAccessFlags, miscFlags));
}
void cgl::CD3D11VertexBuffer::Bind(UINT slot, UINT offset)
{
	UINT stride = GetStride();
	getDevice()->GetContext()->IASetVertexBuffers(slot, 1, ptr(), &stride, &offset );
}
void cgl::CD3D11VertexBuffer::Draw( UINT offset, UINT count)
{
	getDevice()->GetContext()->Draw(count, offset);
}
void cgl::CD3D11VertexBuffer::Draw()
{
	Draw(0, m_desc.ByteWidth / m_stride);
}
void cgl::CD3D11VertexBuffer::Draw( UINT offset )
{
	Draw(offset, m_desc.ByteWidth / m_stride);
}

//////////////////////////////////////////////////////////////////////////
// d3d11 index buffer
cgl::CD3D11IndexBuffer::CD3D11IndexBuffer( UINT elementSize, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, UINT cpuAccessFlags /*= 0*/, UINT miscFlags /*= 0*/ )
	: CD3D11Buffer(elementSize, CD3D11_BUFFER_DESC(0, D3D11_BIND_INDEX_BUFFER, usage, cpuAccessFlags, miscFlags, elementSize ))
{

}
void cgl::CD3D11IndexBuffer::Bind(UINT offset)
{
	switch(GetStride())
	{
	case sizeof(DWORD): getDevice()->GetContext()->IASetIndexBuffer(get(), DXGI_FORMAT_R32_UINT, offset); break;
	case sizeof(WORD):	getDevice()->GetContext()->IASetIndexBuffer(get(), DXGI_FORMAT_R16_UINT, offset); break;
	}

}
void cgl::CD3D11IndexBuffer::Draw( UINT indexOffset, UINT vertexOffset)
{
	Draw(indexOffset, m_desc.ByteWidth / m_stride, vertexOffset);
}
void cgl::CD3D11IndexBuffer::Draw()
{
	Draw(0, m_desc.ByteWidth / m_stride, 0);
}
void cgl::CD3D11IndexBuffer::Draw( UINT indexOffset, UINT indexCount, UINT vertexOffset )
{
	getDevice()->GetContext()->DrawIndexed(indexCount, indexOffset, vertexOffset);
}
std::tr1::shared_ptr<cgl::CD3D11IndexBuffer> cgl::CD3D11IndexBuffer::Create( UINT elementSize, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, UINT cpuAccessFlags /*= 0*/, UINT miscFlags /*= 0*/ )
{
	return create<CD3D11IndexBuffer>(new CD3D11IndexBuffer(elementSize, usage, cpuAccessFlags, miscFlags));
}

//////////////////////////////////////////////////////////////////////////
// d3d11 constant buffer
cgl::CD3D11ConstantBuffer::CD3D11ConstantBuffer( UINT elementSize, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, UINT cpuAccessFlags /*= 0*/, UINT miscFlags /*= 0*/ )
	: CD3D11Buffer(elementSize, CD3D11_BUFFER_DESC(0, D3D11_BIND_CONSTANT_BUFFER, usage, cpuAccessFlags, miscFlags, elementSize ))
{

}
void cgl::CD3D11ConstantBuffer::Bind(CGL_SHADER_STAGE stage, UINT slot)
{
	switch(stage)
	{
	case CGL_SHADER_STAGE_VERTEX: 	getDevice()->GetContext()->VSSetConstantBuffers(slot, 1, ptr()); break;
	case CGL_SHADER_STAGE_PIXEL: 	getDevice()->GetContext()->PSSetConstantBuffers(slot, 1, ptr()); break;
	case CGL_SHADER_STAGE_GEOMETRY: getDevice()->GetContext()->GSSetConstantBuffers(slot, 1, ptr()); break;
	case CGL_SHADER_STAGE_HULL: 	getDevice()->GetContext()->HSSetConstantBuffers(slot, 1, ptr()); break;
	case CGL_SHADER_STAGE_DOMAIN: 	getDevice()->GetContext()->DSSetConstantBuffers(slot, 1, ptr()); break;
	}
}
std::tr1::shared_ptr<cgl::CD3D11ConstantBuffer> cgl::CD3D11ConstantBuffer::Create( UINT elementSize, D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/, UINT cpuAccessFlags /*= 0*/, UINT miscFlags /*= 0*/ )
{
	return create<CD3D11ConstantBuffer>(new CD3D11ConstantBuffer(elementSize, usage, cpuAccessFlags, miscFlags));
}

