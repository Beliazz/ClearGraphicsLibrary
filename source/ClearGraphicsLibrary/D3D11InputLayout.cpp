#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// d3d11 input layout from shader
cgl::CD3D11InputLayout::CD3D11InputLayout( PD3D11EffectPass pPass, ICGLInstancing* pInstancingInfo)
	: CGLBase("CD3D11InputLayout"), m_pPass(pPass), m_pInstancingInfo(pInstancingInfo), m_vertexSize(0)
{

}
HRESULT cgl::CD3D11InputLayout::onRestore(  )
{
	//////////////////////////////////////////////////////////////////////////
	// reset
	m_cglElements.clear();

	//////////////////////////////////////////////////////////////////////////
	// get shader
	D3DX11_PASS_SHADER_DESC vertexShaderDesc;
	ZeroMemory(&vertexShaderDesc, sizeof(vertexShaderDesc));
	HRESULT hResult = m_pPass->get()->GetVertexShaderDesc(&vertexShaderDesc);
	if (FAILED(hResult)) return hResult;

	ID3DX11EffectShaderVariable* pShader = vertexShaderDesc.pShaderVariable->AsShader();

	//////////////////////////////////////////////////////////////////////////
	// get input element count
	D3DX11_EFFECT_SHADER_DESC shaderDesc;
	ZeroMemory(&shaderDesc, sizeof(shaderDesc));
	hResult = pShader->GetShaderDesc(vertexShaderDesc.ShaderIndex, &shaderDesc);
	if (FAILED(hResult)) return hResult;

	//////////////////////////////////////////////////////////////////////////
	// process input elements
	D3D11_INPUT_ELEMENT_DESC* pDesc = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.NumInputSignatureEntries];
	ZeroMemory(pDesc, sizeof(D3D11_INPUT_ELEMENT_DESC) * shaderDesc.NumInputSignatureEntries );
	for (UINT i = 0; i < shaderDesc.NumInputSignatureEntries; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC signatureDesc;
		ZeroMemory(&signatureDesc, sizeof(signatureDesc));
		hResult = pShader->GetInputSignatureElementDesc(vertexShaderDesc.ShaderIndex, i, &signatureDesc );
		if (FAILED(hResult)) return hResult;

		if(signatureDesc.Mask == 0 /*|| signatureDesc.ComponentType == D3D_REGISTER_COMPONENT_UNKNOWN*/)
			return E_FAIL;
		 
		//////////////////////////////////////////////////////////////////////////
		// construct cgl element for application use
		CGL_INPUT_ELEMENT_SIGNATURE cglElement;
		cglElement.semanticIndex = signatureDesc.SemanticIndex;
		cglElement.semanticName =  signatureDesc.SemanticName;

		pDesc[i].SemanticName = signatureDesc.SemanticName;
		pDesc[i].SemanticIndex = signatureDesc.SemanticIndex;
		pDesc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		
		switch(signatureDesc.Mask)
		{
		// 1 component
		case 1:
			{
				switch(signatureDesc.ComponentType)
				{
				case D3D_REGISTER_COMPONENT_FLOAT32:	{ pDesc[i].Format = DXGI_FORMAT_R32_FLOAT; }	break;
				case D3D_REGISTER_COMPONENT_SINT32:		{ pDesc[i].Format = DXGI_FORMAT_R32_SINT; }		break;
				case D3D_REGISTER_COMPONENT_UINT32:		{ pDesc[i].Format = DXGI_FORMAT_R32_UINT; }		break;
				case D3D_REGISTER_COMPONENT_UNKNOWN:	{ pDesc[i].Format = DXGI_FORMAT_R32_TYPELESS; } break;
				}

				// element size
				cglElement.size = ( 1 * 32 ) / 8;

			} break;

		// 2 components ( 2^2 )
		case (4 - 1):
			{ 
				switch(signatureDesc.ComponentType)
				{
				case D3D_REGISTER_COMPONENT_FLOAT32:	{ pDesc[i].Format = DXGI_FORMAT_R32G32_FLOAT; }		break;
				case D3D_REGISTER_COMPONENT_SINT32:		{ pDesc[i].Format = DXGI_FORMAT_R32G32_SINT; }		break;
				case D3D_REGISTER_COMPONENT_UINT32:		{ pDesc[i].Format = DXGI_FORMAT_R32G32_UINT; }		break;
				case D3D_REGISTER_COMPONENT_UNKNOWN:	{ pDesc[i].Format = DXGI_FORMAT_R32G32_TYPELESS; }	break;
				}

				// element size
				cglElement.size = ( 2 * 32 ) / 8;

			} break;
		// 3 components ( 2^3 )
		case (8 - 1):
			{
				switch(signatureDesc.ComponentType)
				{
				case D3D_REGISTER_COMPONENT_FLOAT32:	{ pDesc[i].Format = DXGI_FORMAT_R32G32B32_FLOAT; }		break;
				case D3D_REGISTER_COMPONENT_SINT32:		{ pDesc[i].Format = DXGI_FORMAT_R32G32B32_SINT; }		break;
				case D3D_REGISTER_COMPONENT_UINT32:		{ pDesc[i].Format = DXGI_FORMAT_R32G32B32_UINT; }		break;
				case D3D_REGISTER_COMPONENT_UNKNOWN:	{ pDesc[i].Format = DXGI_FORMAT_R32G32B32_TYPELESS; }	break;
				}

				// element size
				cglElement.size = ( 3 * 32 ) / 8;

			} break;

		// 4 components ( 2^4 )
		case (16 - 1):
			{
				switch(signatureDesc.ComponentType)
				{
				case D3D_REGISTER_COMPONENT_FLOAT32:	{ pDesc[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; }		break;
				case D3D_REGISTER_COMPONENT_SINT32:		{ pDesc[i].Format = DXGI_FORMAT_R32G32B32A32_SINT; }		break;
				case D3D_REGISTER_COMPONENT_UINT32:		{ pDesc[i].Format = DXGI_FORMAT_R32G32B32A32_UINT; }		break;
				case D3D_REGISTER_COMPONENT_UNKNOWN:	{ pDesc[i].Format = DXGI_FORMAT_R32G32B32A32_TYPELESS; }	break;
				}

				// element size
				cglElement.size = ( 4 * 32 ) / 8;
				
			} break;

		default:
			return E_FAIL;
		}

		m_vertexSize += cglElement.size;

		// instancing
		if (m_pInstancingInfo)
		{
			UINT dataStep = 0;
			UINT bufferSlot = 0;
			m_pInstancingInfo->getElementInstancingInfo(signatureDesc.SemanticName, signatureDesc.SemanticIndex, &dataStep, &bufferSlot);
			if(dataStep != 0)
			{
				pDesc[i].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
				pDesc[i].InputSlot = bufferSlot;
				pDesc[i].InstanceDataStepRate = dataStep;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// add input element
		m_cglElements.push_back(cglElement);
	}

	//////////////////////////////////////////////////////////////////////////
	// create input layout
	D3DX11_PASS_DESC passDesc;
	ZeroMemory(&passDesc, sizeof(passDesc));
	hResult = m_pPass->get()->GetDesc(&passDesc);
	if (FAILED(hResult)) return hResult;

	hResult = getDevice()->GetDevice()->CreateInputLayout(pDesc, shaderDesc.NumInputSignatureEntries, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, ptr());

	SAFE_DELETE_ARRAY(pDesc);

	return hResult;
}
void cgl::CD3D11InputLayout::onReset()
{
	comReset((IUnknown**)ptr());
}

void cgl::CD3D11InputLayout::getDependencies(std::vector<cgl::PCGLObject>* pDependencies)
{
	pDependencies->push_back(m_pPass);
}
cgl::PD3D11InputLayout cgl::CD3D11InputLayout::Create( PD3D11EffectPass pPass, ICGLInstancing* pInstancingInfo)
{
	return create<CD3D11InputLayout>(new CD3D11InputLayout(pPass, pInstancingInfo));
}
void cgl::CD3D11InputLayout::Bind()
{
	getDevice()->GetContext()->IASetInputLayout(get());
}
cgl::CD3D11InputLayout::~CD3D11InputLayout()
{
	m_cglElements.clear();
}

bool cgl::CD3D11InputLayout::operator==( CD3D11InputLayout& rhs )
{
	if (this->m_cglElements.size() != rhs.m_cglElements.size())
		return false;

	for (UINT i = 0; i < GetInputElements().size(); i++)
	{
		if (m_cglElements[i] != rhs.m_cglElements[i])
			return false;
	}

	return false;
}


