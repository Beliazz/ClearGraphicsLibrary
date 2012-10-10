#pragma once

#include "cgl.h"

namespace cgl {

class CGLTimer;
typedef std::shared_ptr<CGLTimer> PCGLTimer;

class CGL_API CGLTimer
{
private:
	float m_elapsed;
	std::string m_name;

protected:
	CGLTimer(std::string name) : m_elapsed(0.0f), m_name(name) {};

	void _set(float elapsed) { m_elapsed = elapsed; }
	static PCGLTimer _register(PCGLTimer timer);

public:
	virtual ~CGLTimer();

	virtual void Start() PURE;
	virtual void Stop() PURE;

	inline float get()			 { return m_elapsed; }
	inline std::string getName() { return m_name; }	
};
class CGL_API CGLTimerDatabase
{
private:
	std::map<std::string, PCGLTimer> m_timer;
	CGLTimerDatabase() { }

public:
	static CGLTimerDatabase* Get();

	void AddTimer(PCGLTimer pTimer);
	void DeleteTimer( std::string name );
	PCGLTimer GetTimer(std::string name);

	inline std::map<std::string, PCGLTimer>& GetTimers() { return m_timer; }
};
class CGL_API CGLTimerReport
{
private:
	std::map<std::string, PCGLTimer> m_timer;

public:
	CGLTimerReport() : m_timer(CGLTimerDatabase::Get()->GetTimers()) { }
	inline std::map<std::string, PCGLTimer>* operator ->()	{ return &m_timer; }
	inline std::map<std::string, PCGLTimer>* operator *()	{ return &m_timer; }
};

class CGL_API CGLCpuTimer : public CGLTimer
{
private:
	UINT64 m_start;
	UINT64 m_end;
	UINT64 m_frequency;

protected:
	CGLCpuTimer(std::string name);

public:
	static PCGLTimer Create(std::string name);
	~CGLCpuTimer();

	void Start();
	void Stop();
};
class CGL_API CGLGpuTimer : public CGLTimer, public CGLManagerConnector
{
	PD3D11Query m_pTimeQuery;
	PD3D11Query m_pDisjointQuery;

	UINT64 m_start;
	UINT64 m_end;

protected:
	CGLGpuTimer(std::string name);

public:
	static PCGLTimer Create(std::string name);

	void Start();
	void Stop();
};

}