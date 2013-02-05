#include "CGLMesh.h"

using namespace cgl::util;

const char* cIndicesKey = "INDICES";

// clg element stream
void cgl::util::CGLMeshData::AddStream( std::string key, UINT byteWidth, UINT stride, char* pStream )
{
	assert( (byteWidth / stride) != m_elementCount	 && "invalid element count");
	assert( (m_streams.find(key) != m_streams.end()) && "stream already exists");

	CGLElementStream stream;
	stream.byteWidth = byteWidth;
	stream.stride = stride;
	stream.key = key;
	stream.pStream = pStream;

	m_streams.insert(make_pair(key, stream));
	m_elementSize += stream.stride;
}
void cgl::util::CGLMeshData::RemoveStream( std::string key )
{
	auto it = m_streams.find(key);
	if(it != m_streams.end())
	{
		m_elementSize -= (*it).second.stride;
		m_streams.erase(it);
	}
}

// cgl mesh
cgl::util::PCGLMesh cgl::util::CGLMesh::Create( const CGLMeshData& data, cgl::core::PD3D11Effect pEffect )
{
	return PCGLMesh(new CGLMesh(m_meshData, pEffect));
}
CGLMesh::CGLMesh(const CGLMeshData& data, cgl::core::PD3D11Effect pEffect)
	: m_meshData(data), m_pEffect(pEffect), m_vertexBuffer(nullptr), m_indexBuffer(nullptr)
{

}
CGLMesh::~CGLMesh(void)
{

}

bool cgl::util::CGLMesh::Restore()
{
	std::vector<CGLElementStream> streams;
	m_pPass = nullptr;
	for (UINT cursor = 0; cursor < m_pEffect->Techniques(); cursor++)
	{
		cgl::core::PD3D11EffectTechnique pTechnique = cgl::core::CD3D11EffectTechniqueFromIndex::Create(m_pEffect, cursor);
		cgl::core::PD3D11EffectPass pPass = cgl::core::CD3D11EffectPassFromIndex::Create(pTechnique, 0);
		cgl::core::PD3D11InputLayout pInputLayout = cgl::core::CD3D11InputLayout::Create(pPass);

		bool compatiblePassFound = true;
		for (auto it = pInputLayout->GetInputElements().begin(); it != pInputLayout->GetInputElements().end(); it++)
		{
			if(!m_meshData.IsStreamAvailable(it->semanticName))
			{
				compatiblePassFound = false;
				break;
			}
		}

		if(compatiblePassFound)
		{
			m_pPass = pPass;
			m_pInputLayout = pInputLayout;
			break;
		}
	}

	if(!m_pPass)
		return false;

	for (auto it = m_pInputLayout->GetInputElements().rbegin(); it != m_pInputLayout->GetInputElements().rend(); it++)
	{
		streams.push_back(m_meshData.GetStreams().find((*it).semanticName)->second);
	}

	char* pVertices = new char[m_meshData.GetElementCount() * m_meshData.GetElementSize()];
	for (UINT cursor = 0; cursor < m_meshData.GetElementCount(); cursor++)
	{
		for (auto it = streams.begin(); it != streams.end(); it++)
		{
			memcpy(&pVertices[cursor * m_meshData.GetElementSize()], &((*it).pStream[cursor * (*it).stride]), (*it).stride);
		}
	}

	m_vertexBuffer = cgl::core::CD3D11VertexBuffer::Create(m_meshData.GetElementSize(), D3D11_USAGE_DEFAULT);
	CGLTraceObj(m_vertexBuffer);
	m_vertexBuffer->AddData(pVertices, m_meshData.GetElementCount());
	if(!m_vertexBuffer->restore())
		return false;

	SAFE_DELETE(pVertices);

	auto it = m_meshData.GetStreams().find(cIndicesKey);
	if(it != m_meshData.GetStreams().end())
	{
		m_indexBuffer = cgl::core::CD3D11IndexBuffer::Create((*it).second.stride);
		CGLTraceObj(m_indexBuffer);
		m_indexBuffer->AddData(it->second.pStream, m_meshData.GetElementCount());
		if(!m_indexBuffer->restore())
			return false;
	}

	return true;
}
void cgl::util::CGLMesh::Draw()
{
	m_vertexBuffer->Bind();
	m_indexBuffer->Bind();
	m_pPass->Apply();
	m_indexBuffer->Draw();
}


