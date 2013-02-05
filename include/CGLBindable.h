#pragma once

#include "cgl.h"

namespace cgl
{
	namespace core 
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
			CGL_BIND_INPUTLAYOUT,

			CGL_BIND_UNUSED
		};

		//////////////////////////////////////////////////////////////////////////
		// bind able interface
		// 
		class CGL_API CGLBindable
		{
		friend class CD3D11Device;
		friend class CGLManagerBase;
		friend class CGLManager;
		friend class cgl::debug::CGLManagerDbg;

		private:
			bool m_upToDate__;
			std::vector<void*> m_params;

		protected:
			virtual void bind()	PURE;
			virtual CGL_BIND getBind() PURE;
			virtual UINT getId() PURE;

			inline bool valid()		 { return m_upToDate__; }
			inline void validate()	 { m_upToDate__ = true;  }

		public:
			CGLBindable();
			~CGLBindable();

			template <class T>
			void SetParam(UINT index, T val)
			{
				if(index >= m_params.size())
				{
					m_params.resize(index);
				}

				if (!m_params[index])
				{
					m_params[index] = malloc(sizeof(T));
				}

				memcpy(m_params[index], &val, sizeof(T));
			}

			template <class T>
			inline T& GetParam(UINT param) { return *reinterpret_cast<T*>(m_params.at(param)); }

			void Bind();
			
			inline void invalidate() { m_upToDate__ = false; }
		};

		//////////////////////////////////////////////////////////////////////////
		// generic base class
		template <class T>
		class CGL_API CGLTypedBindable : public CGLBindable
		{
		protected:
			CGLBase<T>* m_pObject__;
			inline UINT getId()  { return m_pObject__->getLuid(); }

		public:
			CGLTypedBindable(CGLBase<T>* pObject)
				: m_pObject__(pObject), CGLBindable()
			{

			}
			virtual ~CGLTypedBindable() { }
		};

		typedef std::tr1::shared_ptr<CGLBindable> PCGLBindable;
	}
}