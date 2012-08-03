#ifndef D3D11Texture_h__
#define D3D11Texture_h__

#include "cgl.h"

namespace cgl
{

//////////////////////////////////////////////////////////////////////////
// typedefs
class D3D11Resource;
class CD3D11TextureFromFile;
class CD3D11TextureFromMemory;
class CD3D11Texture2DBlank;
class CD3D11BackBuffer;
typedef CGL_API std::tr1::shared_ptr<D3D11Resource> PD3D11Resource; 
typedef CGL_API std::tr1::shared_ptr<CD3D11TextureFromFile> PD3D11TextureFromFile; 
typedef CGL_API std::tr1::shared_ptr<CD3D11TextureFromMemory> PD3D11TextureFromMemory; 
typedef CGL_API std::tr1::shared_ptr<CD3D11Texture2DBlank> PD3D11Texture2DBlank; 
typedef CGL_API std::tr1::shared_ptr<CD3D11BackBuffer> PD3D11BackBuffer; 

//////////////////////////////////////////////////////////////////////////
// general resource class
//////////////////////////////////////////////////////////////////////////
class CGL_API D3D11Resource : public CGLBase<ID3D11Resource>
{
protected:
	void onReset();
	void getDependencies(std::vector<PCGLObject>* pDependencies ) { }

public:
	D3D11Resource(std::string className);
};

//////////////////////////////////////////////////////////////////////////
// textures that are loaded from a file
class CGL_API CD3D11TextureFromFile : public D3D11Resource
{
private:
	D3DX11_IMAGE_LOAD_INFO m_loadInfo;
	std::string m_filename;

protected:
	CD3D11TextureFromFile(std::string filename, D3DX11_IMAGE_LOAD_INFO loadInfo);
	HRESULT onRestore();

public:
	static PD3D11Resource Create(std::string filename, D3DX11_IMAGE_LOAD_INFO loadInfo);
};

//////////////////////////////////////////////////////////////////////////
// textures that are loaded from memory
class CGL_API CD3D11TextureFromMemory : public D3D11Resource
{
private:
	D3DX11_IMAGE_LOAD_INFO m_loadInfo;
	void* m_pBuffer;
	size_t m_size;

protected:
	CD3D11TextureFromMemory(void* pBuffer, size_t size, D3DX11_IMAGE_LOAD_INFO loadInfo);
	HRESULT onRestore();

public:
	static PD3D11Resource Create(void* pBuffer, size_t size, D3DX11_IMAGE_LOAD_INFO loadInfo);
};

//////////////////////////////////////////////////////////////////////////
// blank textures
class CGL_API CD3D11Texture2DBlank : public D3D11Resource
{
private:
	D3D11_TEXTURE2D_DESC m_desc;

protected:
	CD3D11Texture2DBlank(D3D11_TEXTURE2D_DESC desc);
	HRESULT onRestore();

public:
	static PD3D11Resource Create(D3D11_TEXTURE2D_DESC desc);
};

//////////////////////////////////////////////////////////////////////////
// for accessing the back buffer
class CGL_API CD3D11BackBuffer : public D3D11Resource
{
protected:
	CD3D11BackBuffer();
	HRESULT onRestore();

public:
	static PD3D11Resource Create();
};

}

#endif // D3D11Texture_h__
