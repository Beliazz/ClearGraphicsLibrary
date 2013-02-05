#ifndef CGLLogger_h__
#define CGLLogger_h__

namespace cgl
{
	namespace debug
	{
		#define CGL_LOG_BUFFER 4096

		class CGL_API CGLLogger
		{
			friend class CGLManagerDbg;
			friend class CGLObject;

		private:
			static void LogObjectState(UINT logType, cgl::core::CGLObject* pObject, HRESULT result = S_OK, void* pData = NULL);

		public:
			static bool Print(LPCSTR log, ...);
		};
	}
}

#endif // CGLLogger_h__
