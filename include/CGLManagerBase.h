#include "cgl.h"

namespace cgl
{
	namespace core
	{
		class CGLObject;
		typedef CGL_API std::tr1::shared_ptr<CGLObject> PCGLObject;
		typedef CGL_API std::list<PCGLObject> PCGLObjectList;

		//////////////////////////////////////////////////////////////////////////
		// manager base class
		class CGL_API CGLManagerBase
		{
			friend class CGLObject;
			friend class CD3D11Device;
			friend class CGLAccess;

		private:
			static CGLManagerBase* m_pInstance;
			static bool IsGarbage( const std::tr1::shared_ptr<CGLObject>& ptr );

			PCGLObjectList m_objects;
			PCGLObject m_pDevice;

		protected:
			virtual bool Register(PCGLObject pObject)	PURE;
			virtual void Unregister(PCGLObject pObject)	PURE;
			virtual bool Reset(CGLObject* pObject)		PURE;
			virtual bool Restore(CGLObject* pObject)	PURE;
			virtual bool RegisterDevice(PCGLObject pObject)	PURE;

			inline PCGLObjectList& GetObjects()			{ return m_objects; }
			inline void SetDevice(PCGLObject pDevice)	{ m_pDevice = pDevice; }

			CGLManagerBase();
			static std::tr1::shared_ptr<CGLManagerBase> create(CGLManagerBase* pData);

			static CGLManagerBase* GetInstance();
			inline PCGLObject& GetDevice() { return m_pDevice; }

		public:
			virtual bool Restore()	PURE;
			virtual bool Reset()	PURE;
			void Tidy();

			virtual ~CGLManagerBase() {}

		};

		typedef CGL_API std::tr1::shared_ptr<CGLManagerBase> PCGLManager;
	}
}
