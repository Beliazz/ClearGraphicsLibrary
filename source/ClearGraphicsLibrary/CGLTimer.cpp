#include "cgl.h"

// cgl timer
cgl::CGLTimer::~CGLTimer()
{
	CGLTimerDatabase::Get()->DeleteTimer(m_name);
}

// cpu timer
cgl::CGLCpuTimer::CGLCpuTimer(std::string name)
	: CGLTimer(name), m_start(0), m_end(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency); 
}
cgl::PCGLTimer cgl::CGLCpuTimer::Create(std::string name)
{
	return cgl::PCGLTimer(_register(new cgl::CGLCpuTimer(name)));
}
void cgl::CGLCpuTimer::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_start);
}
void cgl::CGLCpuTimer::Stop()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_end);
	_set((m_end-m_start) / (float)m_frequency);
}

// gpu timer
cgl::CGLGpuTimer::CGLGpuTimer(std::string name)
	: CGLTimer(name)
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
cgl::PCGLTimer cgl::CGLGpuTimer::Create(std::string name)
{
	return cgl::PCGLTimer(_register(new cgl::CGLGpuTimer(name)));
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
		_set((m_end - m_start) / (float)disjointData.Frequency);
	} 
	else
	{
		_set(-1.0f);
	}
}

// timer database
cgl::CGLTimerDatabase* cgl::CGLTimerDatabase::Get()
{
	static CGLTimerDatabase dataBase;
	return &dataBase;
}
void cgl::CGLTimerDatabase::AddTimer( CGLTimer* pTimer )
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
void cgl::CGLTimerDatabase::DeleteTimer( std::string name )
{
	auto it = m_timer.find(name);
	if (it != m_timer.end())
	{
		m_timer.erase(it);
	}
}
cgl::CGLTimer* cgl::CGLTimerDatabase::GetTimer( std::string name )
{
	auto it = m_timer.find(name);
	if (it != m_timer.end())
	{
		return it->second;
	}

	return NULL;
}
cgl::CGLTimer* cgl::CGLTimer::_register( CGLTimer* pTimer )
{
	CGLTimerDatabase::Get()->AddTimer(pTimer);
	return pTimer;
}


