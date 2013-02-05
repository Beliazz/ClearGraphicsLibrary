#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// d3d11 effect base class
cgl::core::D3D11Effect::D3D11Effect( std::string className ) 
	: CGLBase(className)
{

}
void cgl::core::D3D11Effect::onReset()
{
	comReset((IUnknown**)ptr());
}

UINT cgl::core::D3D11Effect::Techniques()
{
	D3DX11_EFFECT_DESC desc;
	get()->GetDesc(&desc);
	return desc.Techniques;
}
UINT cgl::core::D3D11Effect::ConstantBuffers()
{
	D3DX11_EFFECT_DESC desc;
	get()->GetDesc(&desc);
	return desc.ConstantBuffers;
}

//////////////////////////////////////////////////////////////////////////
// d3d11 effect from memory
cgl::core::CD3D11EffectFromMemory::CD3D11EffectFromMemory(void* pBuffer, SIZE_T size )
	: D3D11Effect("CD3D11EffectFromMemory"), m_size(size)
{
	m_pBuffer = std::tr1::shared_ptr<void>(malloc(size));
	memcpy(m_pBuffer.get(), pBuffer, size);
}
HRESULT cgl::core::CD3D11EffectFromMemory::onRestore( )
{
	return D3DX11CreateEffectFromMemory(m_pBuffer.get(), m_size, NULL, CGLAccess::D3DDevice(), ptr()); 
}
cgl::core::PD3D11Effect cgl::core::CD3D11EffectFromMemory::Create(void* pBuffer, SIZE_T size )
{
	return create(new CD3D11EffectFromMemory(pBuffer, size));
}

//////////////////////////////////////////////////////////////////////////
// d3d11 effect from file
cgl::core::CD3D11EffectFromFile::CD3D11EffectFromFile( std::string fileName )
	: D3D11Effect("CD3D11EffectFromFile"), m_fileName(fileName)
{

}
cgl::core::PD3D11Effect cgl::core::CD3D11EffectFromFile::Create( std::string fileName )
{
	return create<D3D11Effect>(new CD3D11EffectFromFile(fileName));
}
HRESULT cgl::core::CD3D11EffectFromFile::onRestore()
{
	std::ifstream myfile;
	int size = 0;
	std::tr1::shared_ptr<char> pData = nullptr;

	myfile.open (m_fileName.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
	if (!myfile.is_open())
		return E_FAIL;

	size = (int)myfile.tellg();
	pData = std::tr1::shared_ptr<char>(new char[size]);
	myfile.seekg(0, std::ios_base::beg);
	myfile.read(pData.get(), size);
	myfile.close();

	return D3DX11CreateEffectFromMemory(pData.get(), size, NULL, CGLAccess::D3DDevice(), ptr()); 
}

//////////////////////////////////////////////////////////////////////////
// d3d11 effect technique
cgl::core::D3D11EffectTechnique::D3D11EffectTechnique(std::string className, PD3D11Effect pEffect, void* pIdentifier, size_t size ) 
	: D3D11EffectObject(className, pEffect, pIdentifier, size)
{

}
cgl::core::D3D11EffectTechnique::~D3D11EffectTechnique()
{
	onReset();
}

UINT cgl::core::D3D11EffectTechnique::Passes()
{
	D3DX11_TECHNIQUE_DESC desc;
	get()->GetDesc(&desc);
	return desc.Passes;
}

//////////////////////////////////////////////////////////////////////////
// d3d11 effect technique from name
HRESULT cgl::core::CD3D11EffectTechniqueFromName::onRestore()
{
	set(m_pDependence->get()->GetTechniqueByName((char*)m_pIdentifier));
	if(!get()->IsValid())
	{
		set(NULL);
		return E_FAIL;
	}

	return S_OK;
}
cgl::core::CD3D11EffectTechniqueFromName::CD3D11EffectTechniqueFromName( PD3D11Effect pEffect, std::string name )
	: D3D11EffectTechnique("CD3D11EffectTechniqueFromName", pEffect, (void*)name.c_str(), ( name.size() + 1 ) * sizeof(char))
{

}
cgl::core::PD3D11EffectTechnique cgl::core::CD3D11EffectTechniqueFromName::Create( PD3D11Effect pEffect, std::string name )
{
	return create(new CD3D11EffectTechniqueFromName(pEffect, name));
}

//////////////////////////////////////////////////////////////////////////
// d3d11 effect technique from index
HRESULT cgl::core::CD3D11EffectTechniqueFromIndex::onRestore()
{
	set(m_pDependence->get()->GetTechniqueByIndex(*((UINT*)m_pIdentifier)));
	if(!get()->IsValid())
	{
		set(NULL);
		return E_FAIL;
	}

	return S_OK;
}
cgl::core::CD3D11EffectTechniqueFromIndex::CD3D11EffectTechniqueFromIndex( PD3D11Effect pEffect, UINT index )
	: D3D11EffectTechnique("CD3D11EffectTechniqueFromIndex", pEffect, (void*)&index, sizeof(UINT))
{

}
cgl::core::PD3D11EffectTechnique cgl::core::CD3D11EffectTechniqueFromIndex::Create( PD3D11Effect pEffect, UINT index )
{
	return create<D3D11EffectTechnique>(new CD3D11EffectTechniqueFromIndex(pEffect, index));
}

//////////////////////////////////////////////////////////////////////////
// d3d11 effect pass
cgl::core::D3D11EffectPass::D3D11EffectPass( std::string className, PD3D11EffectTechnique pTechnique, void* pIdentifier, size_t size )
	: D3D11EffectObject(className, pTechnique, pIdentifier, size)
{

}
void cgl::core::D3D11EffectPass::Apply()
{
	get()->Apply(0, CGLAccess::D3DContext());
}

//////////////////////////////////////////////////////////////////////////
// d3d11 effect pass from name
HRESULT cgl::core::CD3D11EffectPassFromName::onRestore()
{
	set(m_pDependence->get()->GetPassByName((char*)m_pIdentifier));
	if(!get()->IsValid())
	{
		set(NULL);
		return E_FAIL;
	}

	return S_OK;
}
cgl::core::CD3D11EffectPassFromName::CD3D11EffectPassFromName( PD3D11EffectTechnique pTechnique, std::string name )
	: D3D11EffectPass("CD3D11EffectPassFromName", pTechnique, (void*)name.c_str(), ( name.size() + 1 ) * sizeof(char))
{

}
cgl::core::PD3D11EffectPass cgl::core::CD3D11EffectPassFromName::Create( PD3D11EffectTechnique pTechnique, std::string name )
{
	return create(new CD3D11EffectPassFromName(pTechnique, name));
}

//////////////////////////////////////////////////////////////////////////
// d3d11 effect pass from index
HRESULT cgl::core::CD3D11EffectPassFromIndex::onRestore()
{
	set(m_pDependence->get()->GetPassByIndex(*((UINT*)m_pIdentifier)));
	if(!get()->IsValid())
	{
		set(NULL);
		return E_FAIL;
	}

	return S_OK;
}
cgl::core::CD3D11EffectPassFromIndex::CD3D11EffectPassFromIndex( PD3D11EffectTechnique pTechnique, UINT index )
	: D3D11EffectPass("CD3D11EffectPassFromIndex", pTechnique, (void*)&index, sizeof(UINT))
{

}
cgl::core::PD3D11EffectPass cgl::core::CD3D11EffectPassFromIndex::Create( PD3D11EffectTechnique pTechnique, UINT index )
{
	return create(new CD3D11EffectPassFromIndex(pTechnique, index));
}

//////////////////////////////////////////////////////////////////////////
// d3d11 effect variable
cgl::core::D3D11EffectVariable::D3D11EffectVariable(std::string className, PD3D11Effect pEffect, void* pIdentifier, size_t size)
	: D3D11EffectObject(className, pEffect, pIdentifier, size)
{
}
cgl::core::D3D11EffectVariable::~D3D11EffectVariable()
{
	onReset();
}
bool cgl::core::D3D11EffectVariable::CheckType( D3D10_SHADER_VARIABLE_TYPE type )
{
	D3DX11_EFFECT_TYPE_DESC desc;
	get()->GetType()->GetDesc(&desc);
	return (desc.Type == type);
}

//////////////////////////////////////////////////////////////////////////
// d3d11 effect variable from name
HRESULT cgl::core::CD3D11EffectVariableFromName::onRestore()
{
	set(m_pDependence->get()->GetVariableByName((char*)m_pIdentifier));
	if(!get()->IsValid())
	{
		set(NULL);
		return E_FAIL;
	}

	return S_OK;
}
cgl::core::CD3D11EffectVariableFromName::CD3D11EffectVariableFromName( PD3D11Effect pEffect, std::string name )
	: D3D11EffectVariable("CD3D11EffectVariableFromName", pEffect, (void*)name.c_str(), ( name.size() + 1 ) * sizeof(char))
{

}
cgl::core::PD3D11EffectVariable cgl::core::CD3D11EffectVariableFromName::Create( PD3D11Effect pEffect, std::string name )
{
	return create<D3D11EffectVariable>(new CD3D11EffectVariableFromName(pEffect, name));
}

//////////////////////////////////////////////////////////////////////////
// d3d11 effect variable from semantic
HRESULT cgl::core::CD3D11EffectVariableFromSemantic::onRestore()
{
	set(m_pDependence->get()->GetVariableBySemantic((char*)m_pIdentifier));
	if(!get()->IsValid())
	{
		set(NULL);
		return E_FAIL;
	}

	return S_OK;
}
cgl::core::CD3D11EffectVariableFromSemantic::CD3D11EffectVariableFromSemantic( PD3D11Effect pEffect, std::string name )
	: D3D11EffectVariable("CD3D11EffectVariableFromSemantic", pEffect, (void*)name.c_str(), ( name.size() + 1 ) * sizeof(char))
{

}
cgl::core::PD3D11EffectVariable cgl::core::CD3D11EffectVariableFromSemantic::Create( PD3D11Effect pEffect, std::string name )
{
	return create(new CD3D11EffectVariableFromSemantic(pEffect, name));
}

//////////////////////////////////////////////////////////////////////////
// d3d11 effect variable from index
HRESULT cgl::core::CD3D11EffectVariableFromIndex::onRestore()
{
	set(m_pDependence->get()->GetVariableByIndex(*((UINT*)m_pIdentifier)));
	if(!get()->IsValid())
	{
		set(NULL);
		return E_FAIL;
	}

	return S_OK;
}
cgl::core::CD3D11EffectVariableFromIndex::CD3D11EffectVariableFromIndex( PD3D11Effect pEffect, UINT index )
	: D3D11EffectVariable("CD3D11EffectVariableFromIndex", pEffect, (void*)&index, sizeof(UINT))
{

}
cgl::core::PD3D11EffectVariable cgl::core::CD3D11EffectVariableFromIndex::Create( PD3D11Effect pEffect, UINT index )
{
	return create(new CD3D11EffectVariableFromIndex(pEffect, index));
}
