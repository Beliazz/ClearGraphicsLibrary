#include "cgl.h"

cgl::CGLTimer::CGLTimer()
	: m_start(0), m_end(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency); 
}
cgl::PCGLTimer cgl::CGLTimer::Create()
{
	return cgl::PCGLTimer(new cgl::CGLTimer());
}

void cgl::CGLTimer::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_start);
}
float cgl::CGLTimer::Stop()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_end);
	m_elapsed = (m_end-m_start) / (float)m_frequency;
	return m_elapsed;
}
float cgl::CGLTimer::get()
{
	return m_elapsed;
}