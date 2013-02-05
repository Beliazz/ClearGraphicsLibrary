#ifndef D3D11Buffer_h__
#define D3D11Buffer_h__

#include "cgl.h"

namespace cgl
{
	namespace core
	{
		//////////////////////////////////////////////////////////////////////////
		// typedefs
		class CD3D11Buffer;
		class CD3D11IndexBuffer;
		class CD3D11ConstantBuffer;
		class CD3D11VertexBuffer;
		typedef CGL_API std::tr1::shared_ptr<CD3D11Buffer>			PD3D11Buffer;
		typedef CGL_API std::tr1::shared_ptr<CD3D11VertexBuffer>	PD3D11VertexBuffer;
		typedef CGL_API std::tr1::shared_ptr<CD3D11IndexBuffer>		PD3D11IndexBuffer;
		typedef CGL_API std::tr1::shared_ptr<CD3D11ConstantBuffer>	PD3D11ConstantBuffer;

		//////////////////////////////////////////////////////////////////////////
		// d3d11 buffer
		class CGL_API CD3D11Buffer : public CGLBase<ID3D11Buffer>
		{
		private:
			std::vector<char>	m_data;
	
		protected:
			UINT m_stride;
			D3D11_BUFFER_DESC	m_desc;

			HRESULT onRestore();
			void onReset();
			virtual void onParamUpdate() {}
			virtual void getDependencies(std::vector<PCGLObject>* pDependencies ) { }

			CD3D11Buffer(UINT elementSize, D3D11_BUFFER_DESC desc);

		public:
			~CD3D11Buffer();

			// the byte width member of the buffer 
			// desc will be set automatically
			// 
			// for stride use sizeof(UsedVertexType)
			// if the stride and the size of the elements in the 
			// vector do not match restore() and Update() will fail
			static PD3D11Buffer Create(UINT elementSize, D3D11_BUFFER_DESC desc);

			inline bool Update() { return restore(); }

			inline std::vector<char>*	Data()	{ return &m_data;}
			UINT SetData(char* pData, size_t count );
			UINT AddData(char* pData, size_t count );
			UINT AddData(char* pData);		
			void ResetData();

			// element size
			void SetStride(UINT stride);
			inline UINT GetStride()				{ return m_desc.StructureByteStride; }

			size_t GetDataSize();
			size_t GetBufferSize();
		};


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
		class CGL_API CD3D11VertexBuffer : public CD3D11Buffer, public cgl::core::CGLTypedBindable<ID3D11Buffer>
		{	
		protected:
			CD3D11VertexBuffer(UINT stride, D3D11_USAGE usage, UINT cpuAccessFlags, UINT miscFlags);
			void onParamUpdate();

			void bind();
			inline CGL_BIND getBind() { return CGL_BIND_VERTEXBUFFER; }

		public:
			static PD3D11VertexBuffer Create(UINT stride, D3D11_USAGE usage, UINT cpuAccessFlags = 0, UINT miscFlags = 0);

			void Draw();
			void Draw(UINT offset);
			void Draw(UINT offset, UINT count);

			enum BIND_PARAM
			{
				BIND_PARAM_SLOT_UINT,
				BIND_PARAM_OFFSET_UINT,
				BIND_PARAM_STRIDE_UINT
			};
		};

		//////////////////////////////////////////////////////////////////////////
		// index buffer
		class CGL_API CGLIndexBufferBindable : public CGLTypedBindable<ID3D11Buffer>
		{
		protected:


		public:
			CGLIndexBufferBindable(CGLBase<ID3D11Buffer>* pIndexBuffer) 
				: CGLTypedBindable(pIndexBuffer) { }


		};
		class CGL_API CD3D11IndexBuffer :  public CD3D11Buffer, public cgl::core::CGLTypedBindable<ID3D11Buffer>
		{
		protected:
			CD3D11IndexBuffer(UINT elementSize, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccessFlags = 0, UINT miscFlags = 0);
			void onParamUpdate();

			void bind();
			inline CGL_BIND getBind() { return CGL_BIND_INDEXBUFFER; }

		public:
			static PD3D11IndexBuffer Create(UINT elementSize, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccessFlags = 0, UINT miscFlags = 0);
			void Draw();
			void Draw(UINT indexCount, UINT vertexOffset);
			void Draw(UINT indexOffset, UINT indexCount, UINT vertexOffset);

			enum BIND_PARAM
			{
				BIND_PARAM_OFFSET_UINT,
				BIND_PARAM_FORMAT_DXGI_FORMAT
			};
		};

		//////////////////////////////////////////////////////////////////////////
		// constant buffer
		class CGL_API CD3D11ConstantBuffer : public CD3D11Buffer
		{
		protected:
			CD3D11ConstantBuffer(UINT elementSize, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccessFlags = 0, UINT miscFlags = 0);

		public:
			// element size = struct size
			static PD3D11ConstantBuffer Create(UINT elementSize, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccessFlags = 0, UINT miscFlags = 0);
			void Bind(PD3D11EffectVariable pVar);
		};
	}
}

#endif // D3D11Buffer_h__
