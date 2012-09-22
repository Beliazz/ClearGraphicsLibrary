#include "cgl.h"

// cpu timer
cgl::CGLCpuTimer::CGLCpuTimer()
	: CGLTimer(), m_start(0), m_end(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency); 
}
cgl::PCGLTimer cgl::CGLCpuTimer::Create()
{
	return cgl::PCGLTimer(new cgl::CGLCpuTimer());
}

void cgl::CGLCpuTimer::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_start);
}
void cgl::CGLCpuTimer::Stop()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_end);
	set((m_end-m_start) / (float)m_frequency);
}

// gpu timer
cgl::CGLGpuTimer::CGLGpuTimer()
{
	D3D11_QUERY_DESC timeQueryDesc;
	ZeroMemory(&timeQueryDesc, sizeof(timeQueryDesc));
	timeQueryDesc.Query = D3D11_QUERY_TIMESTAMP;
	m_pTimeQuery = cgl::CD3D11Query::Create(timeQueryDesc);
	CGL_RESTORE(m_pTimeQuery);

	D3D11_QUERY_DESC disjointQueryDesc;
	ZeroMemory(&disjointQueryDesc, sizeof(disjointQueryDesc));
	disjointQueryDesc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
	m_pDisjointQuery = cgl::CD3D11Query::Create(disjointQueryDesc);
	CGL_RESTORE(m_pDisjointQuery);
}
cgl::PCGLTimer cgl::CGLGpuTimer::Create()
{
	return cgl::PCGLTimer(new cgl::CGLGpuTimer());
}

void cgl::CGLGpuTimer::Start()
{
	Context()->Begin(m_pDisjointQuery->get());
	Context()->End(m_pTimeQuery->get());

	m_pTimeQuery->GetData(&m_start);
}
void cgl::CGLGpuTimer::Stop()
{
	Context()->End(m_pTimeQuery->get());
	Context()->End(m_pDisjointQuery->get());

	m_pTimeQuery->GetData(&m_end);

	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointData;
	ZeroMemory(&disjointData, sizeof(D3D11_QUERY_DATA_TIMESTAMP_DISJOINT));
	m_pDisjointQuery->GetData(&disjointData);

	if (!disjointData.Disjoint)
	{
		// query data is valid
		// calculate time in seconds
		set((m_end - m_start) / (float)disjointData.Frequency);
	} 
	else
	{
		set(-1.0f);
	}
}

