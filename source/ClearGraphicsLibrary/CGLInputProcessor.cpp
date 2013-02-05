#include "cgl.h"

cgl::util::CGLInputDataProcessor::CGLInputDataProcessor(cgl::core::PD3D11InputLayout pInputLayout, cgl::util::ICGLInputProcessorDataProvider* pDataProvider, UINT elementCount, UINT step, CGL_PROCESS_MODE mode)
	: m_mode(mode), m_pData(NULL), m_dataSize(0), m_elementSize(0), m_isValid(true), m_pInputLayout(pInputLayout), m_pDataProvider(pDataProvider), m_elementCount(elementCount), m_step(step)
{

}
cgl::util::CGLInputDataProcessor::~CGLInputDataProcessor()
{
	SAFE_FREE(m_pData);
}

char* cgl::util::CGLInputDataProcessor::Process()
{
	if (!m_pInputLayout || !m_pDataProvider || m_elementCount == 0)
		return false;

	// get input elements
	m_currElements = m_pInputLayout->GetInputElements();
	m_elementSize = m_pInputLayout->GetElementSize();
	
	// check if the data provider has the required elements
	if(!m_pDataProvider->isCompatible(m_currElements))
		return false;

	// alloc memory
	m_dataSize = m_pInputLayout->GetElementSize() * m_elementCount;
	m_pData = (char*)malloc(m_dataSize);
	ZeroMemory(m_pData, m_dataSize);

	// get data
	UINT cursor = 0;
	switch (m_mode)
	{
	case CGL_PROCESS_MODE_PER_SUB_ELEMENT:
		{
			for (UINT element = 0; element < m_elementCount; element++)
			{
				for (UINT component = 0; component < m_currElements.size(); component++)
				{
					if (m_pDataProvider->getElement(&m_pData[cursor], element, &m_currElements[component], 1))
						cursor +=  m_currElements[component].size;
				}
			}
		} break;

	case CGL_PROCESS_MODE_PER_ELEMENT:
		{
			for (UINT element = 0; element < m_elementCount; element += m_step)
			{
				if (m_pDataProvider->getElement(&m_pData[cursor], element, m_currElements.data(), m_currElements.size() ))
					cursor += m_elementSize * m_step;
			}

		} break;
	}

	// set data size copied
	// this allows to skip elements
	m_dataSize = cursor;

	m_isValid = true;

	return m_pData;
}
bool cgl::util::CGLInputDataProcessor::isCompatible( cgl::core::PD3D11InputLayout& pLayoutToMatch )
{
	return ( m_pInputLayout.get() == pLayoutToMatch.get()); 
}

cgl::util::PCGLInputDataProcessor cgl::util::CGLInputDataProcessor::Create( cgl::core::PD3D11InputLayout pInputLayout, cgl::util::ICGLInputProcessorDataProvider* pDataProvider, UINT elementCount, UINT step, CGL_PROCESS_MODE mode)
{
	return PCGLInputDataProcessor(new CGLInputDataProcessor(pInputLayout, pDataProvider, elementCount, step, mode));
}

