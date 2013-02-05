#include "cgl.h"

// cgl timer
cgl::time::CGLTimer::CGLTimer( std::string name ) : m_elapsed(0.0f), m_name(name)
{
	CGLTimerDatabase::AddTimer(this);
}
cgl::time::CGLTimer::~CGLTimer()
{
	CGLTimerDatabase::DeleteTimer(m_name);
}

// cpu timer
cgl::time::CGLCpuTimer::CGLCpuTimer(std::string name)
	: CGLTimer(name), m_start(0), m_end(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency); 
}
cgl::time::PCGLTimer cgl::time::CGLCpuTimer::Create(std::string name)
{
	return cgl::time::PCGLTimer(new cgl::time::CGLCpuTimer(name));
}
void cgl::time::CGLCpuTimer::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_start);
}
void cgl::time::CGLCpuTimer::Stop()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_end);
	_set((m_end-m_start) / (float)m_frequency);
}

// gpu timer
cgl::time::CGLGpuTimer::CGLGpuTimer(std::string name)
	: CGLTimer(name)
{
	D3D11_QUERY_DESC timeQueryDesc;
	ZeroMemory(&timeQueryDesc, sizeof(timeQueryDesc));
	timeQueryDesc.Query = D3D11_QUERY_TIMESTAMP;
	m_pTimeQuery = cgl::core::CD3D11Query::Create(timeQueryDesc);
	CGL_RESTORE(m_pTimeQuery);

	D3D11_QUERY_DESC disjointQueryDesc;
	ZeroMemory(&disjointQueryDesc, sizeof(disjointQueryDesc));
	disjointQueryDesc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
	m_pDisjointQuery = cgl::core::CD3D11Query::Create(disjointQueryDesc);
	CGL_RESTORE(m_pDisjointQuery);
}
cgl::time::PCGLTimer cgl::time::CGLGpuTimer::Create(std::string name)
{
	return cgl::time::PCGLTimer(new cgl::time::CGLGpuTimer(name));
}
void cgl::time::CGLGpuTimer::Start()
{
	D3DContext()->Begin(m_pDisjointQuery->get());
	D3DContext()->End(m_pTimeQuery->get());

	m_pTimeQuery->GetData(&m_start);
}
void cgl::time::CGLGpuTimer::Stop()
{
	D3DContext()->End(m_pTimeQuery->get());
	D3DContext()->End(m_pDisjointQuery->get());

	m_pTimeQuery->GetData(&m_end);

	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointData;
	ZeroMemory(&disjointData, sizeof(D3D11_QUERY_DATA_TIMESTAMP_DISJOINT));
	m_pDisjointQuery->GetData(&disjointData);

	if (!disjointData.Disjoint)
	{
		// query data is valid
		// calculate time in seconds
		_set((m_end - m_start) / (float)disjointData.Frequency);
	} 
	else
	{
		_set(-1.0f);
	}
}

// timer database
cgl::time::CGLTimerMap cgl::time::CGLTimerDatabase::m_timer = CGLTimerMap();
void cgl::time::CGLTimerDatabase::AddTimer( CGLTimer* pTimer )
{
	auto it = m_timer.find(pTimer->getName());
	if (it != m_timer.end())
	{
		it->second = pTimer;
	}
	else
	{
		m_timer.insert(make_pair(pTimer->getName(), pTimer));
	}
}
void cgl::time::CGLTimerDatabase::DeleteTimer( std::string name )
{
	auto it = m_timer.find(name);
	if (it != m_timer.end())
	{
		m_timer.erase(it);
	}
}
cgl::time::CGLTimer* cgl::time::CGLTimerDatabase::GetTimer( std::string name )
{
	auto it = m_timer.find(name);
	if (it != m_timer.end())
	{
		return it->second;
	}

	return NULL;
}



