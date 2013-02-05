#ifndef D3D11Manager_h__
#define D3D11Manager_h__

#include "cgl.h"

namespace cgl
{
	namespace core
	{ 
		#ifdef CGL_DEBUG
		#define CGL_RESTORE(x) x->restore()
		#define CGL_RESTORE_ALL(CGLMgr) CGLMgr->Restore()
		#else
		#define CGL_RESTORE(x) x->restore()
		#define CGL_RESTORE_ALL(CGLMgr) CGLMgr->Restore()
		#endif

		class CGL_API CGLManager : public cgl::core::CGLManagerBase
		{
		private:
			bool Reset(CGLObject* pObject);
			bool Restore(CGLObject* pObject);
			bool RegisterDevice(PCGLObject pObject);
			bool Register(PCGLObject pObject);
			void Unregister(PCGLObject pObject);

		protected:
			CGLManager();

		public:	
			static PCGLManager Create();
			virtual ~CGLManager();

			bool Reset();
			bool Restore();			
		};
	}
}

#endif // D3D11Manager_h__