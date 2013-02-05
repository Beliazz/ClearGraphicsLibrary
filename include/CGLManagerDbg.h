#pragma once

#include "cgl.h"

namespace cgl
{
	namespace debug
	{ 
		enum CGL_MANAGER_NOTIFICATION
		{
			CGL_NOTIFICATION_INVALID_PTR,
			CGL_NOTIFICATION_NO_DEVICE,

			CGL_NOTIFICATION_INSTANTIATION,
			CGL_NOTIFICATION_INSTANTIATION_FAILED,

			CGL_NOTIFICATION_CREATION,
			CGL_NOTIFICATION_CREATION_FAILED,

			CGL_NOTIFICATION_RESTORATION,
			CGL_NOTIFICATION_RESTORATION_FAILED,

			CGL_NOTIFICATION_REGISTRATION,
			CGL_NOTIFICATION_UNREGISTRATION,
			CGL_NOTIFICATION_REGISTRATION_FAILED,

			CGL_NOTIFICATION_CYCLIC_DEPENDENCY,
			CGL_NOTIFICATION_RESET,
			CGL_NOTIFICATION_DESCTRUCTION,
			CGL_NOTIFICATION_STILL_ALIVE,
			CGL_NOTIFICATION_COM_INTERFACE_STILL_ALIVE,

			CGL_NOTIFICATION_MGR_INITIALIZED,
			CGL_NOTIFICATION_MGR_SHUTDOWN,



			CGL_NOTIFICATION_CUSTOM
		};

		#define CGLTrace(pObject, name) CGLFeatureAccessSafe(pObject, cgl::core::CGL_OBJECT_FEATURE_DEBUG, cgl::debug::CGLDebugFeature)->Trace(__FILE__, __FUNCTION__, __LINE__, name)
	
		typedef void (CGLNotificationCallback)(UINT, cgl::core::CGLObject*, void*);
		typedef CGLNotificationCallback* PCGLNotificationCallback;

		class CGL_API CGLManagerDbg : public cgl::core::CGLManagerBase
		{
		private:
			bool Reset(cgl::core::CGLObject* pObject);
			bool Restore(cgl::core::CGLObject* pObject);
			bool RegisterDevice(cgl::core::PCGLObject pObject);
			bool Register(cgl::core::PCGLObject pObject);
			void Unregister(cgl::core::PCGLObject pObject);

		protected:
			CGLManagerDbg();

		public:	
			static cgl::core::PCGLManager Create();
			~CGLManagerDbg();

			bool Reset();
			bool Restore();	

			void Notify ( UINT notificationType, cgl::core::CGLObject* pObject = NULL, void* pData = NULL );
		};
	}
}