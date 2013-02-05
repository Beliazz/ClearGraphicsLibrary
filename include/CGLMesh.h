#pragma once

#include "cgl.h"

namespace cgl
{
	namespace util
	{
		struct CGLElementStream
		{
			char* pStream;
			std::string key;
			UINT byteWidth;
			UINT stride;
		};

		class CGLMeshData
		{
		private:
			UINT m_elementCount;
			UINT m_elementSize;
			std::map<std::string, CGLElementStream> m_streams;

		public:
			CGLMeshData(UINT elementCount) : m_elementCount(elementCount) { };
			void AddStream(std::string key, UINT byteWidth, UINT stride, char* pStream);
			void RemoveStream(std::string key);

			inline std::map<std::string, CGLElementStream>&	 GetStreams()	{ return m_streams; }
			inline bool IsStreamAvailable(std::string key)					{ return (m_streams.find(key) != m_streams.end()); }
			inline UINT GetElementCount()									{ return m_elementCount; }
			inline UINT GetElementSize()									{ return m_elementSize; }
		};

		typedef std::tr1::shared_ptr<class CGLMesh> PCGLMesh;
		class CGL_API CGLMesh
		{
		private:
			CGLMeshData m_meshData;
			cgl::core::PD3D11Effect m_pEffect;
			cgl::core::PD3D11InputLayout m_pInputLayout;
			cgl::core::PD3D11VertexBuffer m_vertexBuffer;
			cgl::core::PD3D11IndexBuffer m_indexBuffer;
			cgl::core::PD3D11EffectPass m_pPass;

			CGLMesh(const CGLMeshData& data, cgl::core::PD3D11Effect pEffect);

		public:
			PCGLMesh Create(const CGLMeshData& data, cgl::core::PD3D11Effect pEffect);
			~CGLMesh();

			bool Restore();
			void Draw();
			void SetMeshData(const CGLMeshData& data) { m_meshData = m_meshData; }

		}; 
	}
}



