#include "cgl.h"

DWORD cgl::debug::CGLLogListener::m_currId = 0;
std::vector<std::string> cgl::debug::CGLLog::m_funcs = std::vector<std::string>();
cgl::debug::CGLLogListenerList cgl::debug::CGLLog::m_listeners = cgl::debug::CGLLogListenerList();

void cgl::debug::CGLLog::AddEntry( const CGLLogEntry& notification )
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); it++)
	{
		it->second->Process(notification);
	}
}

void cgl::debug::CGLLog::AddListener( CGLLogListener* pListener )
{
	m_listeners.insert(std::make_pair(pListener->GetId(), pListener));
}
void cgl::debug::CGLLog::RemoveListener( CGLLogListener* pListener )
{
	auto it = m_listeners.find(pListener->GetId());
	if (it != m_listeners.end())
	{
		m_listeners.erase(it);
	}
}

void cgl::debug::CGLLog::PushFunction(std::string func )
{
	m_funcs.push_back(func);
	for (auto it = m_listeners.begin(); it != m_listeners.end(); it++)
	{
		it->second->Process(m_funcs.back());
		it->second->Intend(1);
	}	
}
void cgl::debug::CGLLog::PopFunction()
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); it++)
	{
		it->second->Intend(-1);
		it->second->Process(m_funcs.back());
	}

	m_funcs.pop_back();
}

