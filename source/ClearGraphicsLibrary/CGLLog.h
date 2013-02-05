#pragma once

#include "cgl.h"

namespace cgl
{
	namespace core
	{
		class CGLObject;
	}

	namespace debug
	{
				// entry category
		enum CGL_LOG_CATEGORY
		{
			CGL_LOG_CATEGORY_INFO,
			CGL_LOG_CATEGORY_WARNING,
			CGL_LOG_CATEGORY_ERROR,
		};

		class CGL_API CGLLogEntry;
		class CGL_API CGLLogListener
		{
			friend class CGLLog;

		private:
			DWORD m_id;
			static DWORD m_currId;

		protected:
			inline DWORD GetId() { return m_id; }

		public:
			CGLLogListener() : m_id(m_currId++) { }

			virtual void Process(const CGLLogEntry& notification) PURE;
			virtual void Process(const std::string& message) PURE;
			virtual void Intend(int direction) PURE;
		};

		//////////////////////////////////////////////////////////////////////////
		// static log class
		typedef std::map<UINT, CGLLogListener*> CGLLogListenerList;
		class CGL_API CGLLog
		{	
		private:
			friend class CGLStackEntry; // for push and pop function
			friend class CGLLogEntry;

			static CGLLogListenerList m_listeners;
			static std::vector<std::string> m_funcs;

			static void AddEntry(const CGLLogEntry& notification);
			static void PushFunction(std::string func);
			static void PopFunction();

		public:
			static void AddListener(CGLLogListener* pListener);
			static void RemoveListener(CGLLogListener* pListener);
		};		
		
		//////////////////////////////////////////////////////////////////////////
		// entry for logging
		class CGL_API CGLLogEntry
		{
		private:
			UINT m_category;
			UINT m_line;

			std::string m_message;
			std::string m_file;
			
			DWORD m_timeStamp;
			void* m_pData;
			cgl::core::CGLObject* m_pRelatedObj;

		public:
			CGLLogEntry(std::string file, UINT line, UINT category, std::string message = "", cgl::core::CGLObject* pRelatedObj = NULL, void* pData = NULL)
				: m_pRelatedObj(pRelatedObj), m_file(file), m_line(line), m_category(category), m_message(message), m_timeStamp(timeGetTime()), m_pData(pData) { CGLLog::AddEntry(*this); };

			inline const UINT				Category()		const { return m_category; }
			inline const std::string		Message()		const { return m_message; }
			inline const void*				Data()			const { return m_pData; }
			inline cgl::core::CGLObject*	RelatedObj()	const { return m_pRelatedObj; }
			inline const std::string		File()			const { return m_file; }
			inline const UINT				Line()			const { return m_line; }
		};

		//////////////////////////////////////////////////////////////////////////
		// class for automated function tracing (push/pop)
		class CGL_API CGLStackEntry
		{
		public:
			CGLStackEntry(const char* function) { CGLLog::PushFunction(function);  }
			~CGLStackEntry()					{ CGLLog::PopFunction(); }
		};

		#define CGLNotify(category, message)	cgl::debug::CGLLogEntry(__FILE__, __LINE__, category, message)
		#define CGLWarning(message)				CGLNotify(cgl::debug::CGL_LOG_CATEGORY_WARNING, message)
		#define CGLError(message)				CGLNotify(cgl::debug::CGL_LOG_CATEGORY_ERROR, message)
		#define CGLInfo(message)				CGLNotify(cgl::debug::CGL_LOG_CATEGORY_INFO, message)
		#define CGLTraceFunc()					cgl::debug::CGLStackEntry __func__(__FUNCSIG__)
		#define CGLTraceObj(obj)				obj->setName(#obj)

	}
}


