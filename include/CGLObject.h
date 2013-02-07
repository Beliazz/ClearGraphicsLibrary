#ifndef CGLObject_h__
#define CGLObject_h__

#include "cgl.h"

namespace cgl
{
	namespace core 
	{
		//////////////////////////////////////////////////////////////////////////
		// d3d shader stage
		enum CGL_API CGL_SHADER_STAGE
		{
			CGL_SHADER_STAGE_VERTEX,
			CGL_SHADER_STAGE_PIXEL,
			CGL_SHADER_STAGE_GEOMETRY,
			CGL_SHADER_STAGE_HULL,
			CGL_SHADER_STAGE_DOMAIN
		};

		template <class T>
		struct ComPtr
		{
		private:
			T* m_pIUnknown;

		public:
			ComPtr(T* ptr) : m_pIUnknown(ptr) { }
			ComPtr(ComPtr& rhs) : m_pIUnknown(rhs.m_pIUnknown)
			{
				m_pIUnknown->AddRef();
			}
			~ComPtr() { m_pIUnknown->Release(); }

			inline T* operator ->() { return m_pIUnknown; }
			inline T* operator  *() { return m_pIUnknown; }
			inline operator T*() 	{ return m_pIUnknown; }
		};

		//////////////////////////////////////////////////////////////////////////
		// cgl manager connector
		class CD3D11Device;
		class CGL_API CGLAccess
		{
		public:
			static CGLManagerBase*				CGLMgr();
			static CD3D11Device*				CGLDevice();
			static ComPtr<ID3D11DeviceContext>	D3DContext();
			static ID3D11Device* 				D3DDevice();
		};

		//////////////////////////////////////////////////////////////////////////
		// cgl base class 
		class CGL_API CGLObject
		{
		friend class CGLManagerBase;
		friend class CGLManager;
		friend class cgl::debug::CGLManagerDbg;

		private:
			UINT m_luid;
			std::string m_typeName;
			std::string m_name;

			bool m_restored;
			bool m_processing;
			bool m_registered;

			HRESULT m_lastResult;
	
			static UINT currLUID;
	
			inline void	setProcessing(bool processing)	{ m_processing = processing; }
			inline void	setRestored(bool restored)		{ m_restored = restored; }
			inline void	setRegistered(bool registered)	{ m_registered = registered; }
			inline bool processing()					{ return m_processing; }
			inline bool registered()					{ return m_registered; }

			inline void setLastResult(HRESULT hResult)	{ m_lastResult = hResult; }

		protected:	
			void comReset(IUnknown** ppComPtr);

			template <class T>
			static std::tr1::shared_ptr<T> create(T* pInstanceData);
		
			virtual HRESULT onRestore() PURE;
			virtual void onReset() PURE;
			virtual void getDependencies(std::vector<PCGLObject>* pDependencies ) PURE;

		public:
			inline UINT getLuid()				{ return m_luid; }
			inline std::string getTypeName()	{ return m_typeName; }	
			inline bool isRestored()			{ return m_restored; }
			inline HRESULT getLastResult()		{ return m_lastResult; }

			bool restore();
			void reset();

			inline void setName(std::string name) { m_name = name; }
			inline std::string getName() { return m_name; }

			CGLObject(std::string className);
			virtual ~CGLObject();
		};
	
		template <class T>
		static std::tr1::shared_ptr<T> cgl::core::CGLObject::create( T* pData )
		{
			std::tr1::shared_ptr<T> pInstance(pData);

			if (pInstance)
				if(CGLManagerBase::GetInstance()->Register(pInstance))
					return pInstance;

			return nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		// wraps a pointer
		template <class T>
		class CGL_API CGLContainer
		{
		private:
			T* m_pObj;

		public:
			CGLContainer() :  m_pObj(NULL) { }
			CGLContainer(T* obj) : m_pObj(obj) { }

			inline T* get()			{return m_pObj;}
			inline T** ptr()		{return &m_pObj;}
			inline void set(T* obj)	{m_pObj = obj;}

			inline operator T*()	{return m_pObj;}
			inline operator T**()	{return &m_pObj;}
		};

		//////////////////////////////////////////////////////////////////////////
		// combined
		template <class T>
		class CGL_API CGLBase : public CGLObject, public CGLContainer<T>
		{
		public:
			CGLBase(std::string className) : CGLObject(className) { }
		};
	}
}

#endif // CGLObject_h__
