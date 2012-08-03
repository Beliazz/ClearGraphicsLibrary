#ifndef CGLObject_h__
#define CGLObject_h__

#include "cgl.h"

namespace cgl
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

//////////////////////////////////////////////////////////////////////////
// typedef
class CGLObject;
typedef CGL_API std::tr1::shared_ptr<CGLObject> PCGLObject;
typedef CGL_API std::list<PCGLObject> PCGLObjectList;

#define DUMP_VAR(type, name) struct __ident_##name { char identifier[64]; __ident_##name () { char tmp[64] = #name "\0"; strcpy(identifier, tmp); }; }; __ident_##name _##name##_; type name;

//////////////////////////////////////////////////////////////////////////
// cgl base class 
class D3D11Device;
class CGL_API CGLObject
{
friend class CGLManager;

private:
	UINT m_luid;
	std::string m_typeName;
	std::string m_name;
	
	bool m_restored;
	bool m_processing;
	bool m_registered;
	
	static UINT currLUID;
	
	inline void	setProcessing(bool processing)	{ m_processing = processing; }
	inline void	setRestored(bool restored)		{ m_restored = restored; }
	inline void	setRegistered(bool registered)	{ m_registered = registered; }
	inline bool processing()					{ return m_processing; }
	inline bool registered()					{ return m_registered; }

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
	std::tr1::shared_ptr<D3D11Device> getDevice();
	
	void setName(std::string name);
	std::string getName();

	bool restore();
	void reset();

	CGLObject(std::string className);
	virtual ~CGLObject();
	
	// template<class T>
	// 	static void dump(T* pObject)
// 	{
// 		BYTE* pData = (BYTE*)pObject;
// 		UINT pDataSize = sizeof(T);
// 		CGLObject* pParent = dynamic_cast<CGLObject*>(pObject);
// 
// 		std::stringstream dumpStr;
// 
// 		dumpStr << pParent->getTypeName() << " ";
// 		dumpStr << "\"" << pParent->getName() << "\" ";
// 		dumpStr << "(" << pParent->getLuid() << ") -> ";
// 
// 		if (pData)
// 		{		
// 			dumpStr << "private data dump (Address: " << std::hex << "0x" << (void*)pData << ", Size: " << std::dec << pDataSize << " Bytes)" << std::endl;
// 
// 			for (UINT i = 0; i < pDataSize; i += 32 )
// 			{
// 				dumpStr << "      > " << std::dec << std::setw(10) << std::setfill('0') << i << "    ";
// 				if (pDataSize < 32)
// 				{
// 					for (int j = 0; j < pDataSize; j++)
// 					{
// 						dumpStr << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)pData[i + j] << " ";
// 						if (j == 15)
// 							dumpStr << " ";
// 					}	
// 					
// 					for (int i = pDataSize; i < 32; i++)
// 					{
// 						if (i == 15)
// 							dumpStr << " ";
// 
// 						dumpStr << "   ";
// 					}
// 				}
// 				else
// 				{
// 					for (int j = 0; j < 32; j++)
// 					{
// 						dumpStr << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)pData[i + j] << " ";
// 						if (j == 15)
// 							dumpStr << " ";
// 					}
// 				}
// 
// 				std::string string;
// 				if ( pDataSize < 32)
// 				{
// 
// 					string.resize(pDataSize);
// 					memcpy((void*)string.data(), &pData[i], pDataSize);
// 					std::replace(string.begin(), string.end(), '\0', '.');
// 				}
// 				else
// 				{
// 					string.resize(32);
// 					memcpy((void*)string.data(), &pData[i], 32);
// 					std::replace(string.begin(), string.end(), '\0', '.');
// 				}
// 
// 				dumpStr << "    "  << '|' << string.c_str() << '|' << std::endl;
// 
// 			}
// 
// 			int rem = (pDataSize - ((pDataSize / 32) * 32)) % 32;
// 			if (rem != 0)
// 			{
// 				dumpStr << "      > " << std::dec << std::setw(10) << std::setfill('0') << (pDataSize / 32 + 1) * 32 << "    ";
// 				for (int i = 0; i < rem; i++)
// 				{
// 					dumpStr << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)pData[pDataSize / 32 + 1 + i] << " ";
// 					if (i == 15 && rem > 16)
// 						dumpStr << " ";
// 				}
// 
// 				for (int i = rem; i < 32; i++)
// 				{
// 					if (i == 15)
// 						dumpStr << " ";
// 
// 					dumpStr << "   ";
// 				}
// 
// 				std::string string;
// 				string.resize(32);
// 				memcpy((void*)string.data(), &pData[(pDataSize / 32) * 32], rem);
// 				std::replace(string.begin(), string.end(), '\0', '.');
// 
// 				dumpStr << "    "  << '|' << string.c_str() << '|' << std::endl;
// 			}
// 		}
// 		else
// 		{
// 			dumpStr << "private Data is NULL";
// 		}
// 
// 		cgl::CGLLogger::Print(dumpStr.str().c_str());
// 	}
};

template <class T>
static std::tr1::shared_ptr<T> cgl::CGLObject::create( T* pData )
{
	std::tr1::shared_ptr<T> pInstance(pData);

	if (pInstance)
		if(CGLManager::GetInstance()->Register(pInstance))
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

#endif // CGLObject_h__
