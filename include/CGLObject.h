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

		struct D3DContext
		{
		private:
			std::tr1::shared_ptr<ID3D11DeviceContext> m_pContext;

		public:
			D3DContext(ID3D11DeviceContext* pContext) : m_pContext(pContext) { }
			~D3DContext() { m_pContext->Release(); }

			inline ID3D11DeviceContext* operator ->() { return m_pContext.get(); }
			inline ID3D11DeviceContext* operator  *() { return m_pContext.get(); }
			inline operator ID3D11DeviceContext*() 	  { return m_pContext.get(); }
		};

		//////////////////////////////////////////////////////////////////////////
		// cgl manager connector
		class CD3D11Device;
		class CGL_API CGLAccess
		{
		public:
			static CGLManagerBase*	CGLMgr();
			static CD3D11Device*	CGLDevice();
			static D3DContext		D3DContext();
			static ID3D11Device*	D3DDevice();
		};

		//////////////////////////////////////////////////////////////////////////
		// cgl base class 
		class CD3D11Device;
		class CGL_API CGLObject
		{
		friend class CGLManagerBase;
		friend class CGLManager;
		friend class cgl::debug::CGLManagerDbg;
		friend class CGLLogger;

		template <class T>
		friend class CGLTypedBindable;

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
			static std::tr1::shared_ptr<typename T> create(T* pInstanceData);
		
			virtual HRESULT onRestore() PURE;
			virtual void onReset() PURE;
			virtual void getDependencies(std::vector<PCGLObject>* pDependencies ) PURE;
	
			virtual std::string toStringSpecific(std::string indent) { return ""; }

			bool _depends(CGLObject* objectToCheck, CGLObject* possibleDependency);

		public:
			inline UINT getLuid()				{ return m_luid; }
			inline std::string getTypeName()	{ return m_typeName; }	
			inline bool isRestored()			{ return m_restored; }
			inline HRESULT getLastResult()		{ return m_lastResult; }

			bool restore();
			void reset();

			std::string toString(std::string indent = "");

			bool depends(PCGLObject& possibleDependency);
	
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
