#pragma once

#include "cgl.h"

namespace cgl {

class CGLTimer;
typedef std::shared_ptr<CGLTimer> PCGLTimer;

class CGL_API CGLTimer
{
private:
	float m_elapsed;

protected:
	void set(float elapsed) {m_elapsed = elapsed; }
	CGLTimer() : m_elapsed(0.0f) {};

public:
	virtual void Start() PURE;
	virtual void Stop() PURE;
	inline float get() { return m_elapsed; }
};

class CGL_API CGLCpuTimer : public CGLTimer
{
private:
	UINT64 m_start;
	UINT64 m_end;
	UINT64 m_frequency;

	CGLCpuTimer();

public:
	static PCGLTimer Create();

	void Start();
	void Stop();
};

class CGL_API CGLGpuTimer : public CGLTimer, public CGLManagerConnector
{
	PD3D11Query m_pTimeQuery;
	PD3D11Query m_pDisjointQuery;

	UINT64 m_start;
	UINT64 m_end;

	CGLGpuTimer();

public:
	static PCGLTimer Create();

	void Start();
	void Stop();
};


}