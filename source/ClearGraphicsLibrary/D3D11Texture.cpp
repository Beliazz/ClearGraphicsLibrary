#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// texture
cgl::core::D3D11Resource::D3D11Resource( std::string className ) : CGLBase(className)
{ }
void cgl::core::D3D11Resource::onReset()
{
	comReset((IUnknown**)ptr());
}

//////////////////////////////////////////////////////////////////////////
// texture from file
HRESULT cgl::core::CD3D11TextureFromFile::onRestore()
{
	// get image info 
	D3DX11_IMAGE_INFO imgInfo;
	HRESULT result = D3DX11GetImageInfoFromFileA(m_filename.c_str(), NULL, &imgInfo, NULL);
	if (FAILED(result)) { return result;}
	m_loadInfo.pSrcInfo = &imgInfo;

	return D3DX11CreateTextureFromFileA(CGLAccess::D3DDevice(), m_filename.c_str(), &m_loadInfo, NULL, ptr(), NULL);
}
cgl::core::CD3D11TextureFromFile::CD3D11TextureFromFile( std::string filename, D3DX11_IMAGE_LOAD_INFO loadInfo ) : D3D11Resource("CD3D11TextureFromFile"), 
	m_loadInfo(loadInfo),
	m_filename(filename)
{

}
cgl::core::PD3D11Resource cgl::core::CD3D11TextureFromFile::Create( std::string filename, D3DX11_IMAGE_LOAD_INFO loadInfo )
{
	return create(new CD3D11TextureFromFile(filename, loadInfo));
}

//////////////////////////////////////////////////////////////////////////
// texture from memory
HRESULT cgl::core::CD3D11TextureFromMemory::onRestore()
{
	D3DX11_IMAGE_INFO imgInfo;
	HRESULT result  = D3DX11GetImageInfoFromMemory(m_pBuffer, m_size, NULL, &imgInfo, NULL);
	if (FAILED(result))
	{
		return result;
	}

	m_loadInfo.pSrcInfo = &imgInfo;

	return D3DX11CreateTextureFromMemory(CGLAccess::D3DDevice(), m_pBuffer, m_size, &m_loadInfo, NULL, ptr(), NULL);
}
cgl::core::CD3D11TextureFromMemory::CD3D11TextureFromMemory( void* pBuffer, size_t size, D3DX11_IMAGE_LOAD_INFO loadInfo ) :	D3D11Resource("CD3D11TextureFromMemory"), 
	m_loadInfo(loadInfo),
	m_pBuffer(pBuffer),
	m_size(size)
{

}
cgl::core::PD3D11TextureFromMemory cgl::core::CD3D11TextureFromMemory::Create( void* pBuffer, size_t size , D3DX11_IMAGE_LOAD_INFO loadInfo )
{
	return create(new CD3D11TextureFromMemory(pBuffer, size, loadInfo));
}

//////////////////////////////////////////////////////////////////////////
// blank texture
HRESULT cgl::core::CD3D11Texture2DBlank::onRestore( )
{
	ID3D11Texture2D* pTemp = NULL;
	HRESULT result = CGLAccess::D3DDevice()->CreateTexture2D(&m_desc, NULL, &pTemp);
	if(SUCCEEDED(result)){ set((ID3D11Resource*) pTemp);}

	return result;
}
cgl::core::CD3D11Texture2DBlank::CD3D11Texture2DBlank( D3D11_TEXTURE2D_DESC desc ) : D3D11Resource("CD3D11Texture2DBlank"), m_desc(desc)
{

}
cgl::core::PD3D11Texture2DBlank cgl::core::CD3D11Texture2DBlank::Create( D3D11_TEXTURE2D_DESC desc )
{
	return create(new CD3D11Texture2DBlank(desc));
}

//////////////////////////////////////////////////////////////////////////
// back buffer texture
cgl::core::CD3D11BackBuffer::CD3D11BackBuffer(PDXGISwapChain pSwapChain) 
	: D3D11Resource("CD3D11BackBuffer"), m_pSwapChain(pSwapChain)
{

}
HRESULT cgl::core::CD3D11BackBuffer::onRestore( )
{
	return m_pSwapChain->get()->GetBuffer(0, __uuidof(ID3D11Texture2D*), reinterpret_cast<void**>(ptr()));
}
cgl::core::PD3D11BackBuffer cgl::core::CD3D11BackBuffer::Create(PDXGISwapChain pSwapChain)
{
	return create(new CD3D11BackBuffer(pSwapChain));
}
void cgl::core::CD3D11BackBuffer::onReset()
{
	SAFE_RELEASE((*ptr()));
}

void cgl::core::CD3D11BackBuffer::getDependencies( std::vector<PCGLObject>* pDependencies )
{
	pDependencies->push_back(m_pSwapChain);
}

