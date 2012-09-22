#pragma once

#include "cgl.h"

namespace cgl {

//////////////////////////////////////////////////////////////////////////
// Smoother
template<typename T> 
class CGL_API Smoother
{
private:
	T*		m_pBuffer;
	T		m_stdVal;
	UINT	m_filterSize;
	UINT	m_cursor;
public:
	Smoother(int filterSize, const T& stdVal) 
		:  m_cursor(0), m_filterSize(filterSize), m_stdVal(stdVal)
		{
			m_pBuffer = new T[m_filterSize];
		}
		~Smoother(void) { SAFE_DELETE_ARRAY(m_pBuffer);}

	void Add(const T& value) {
		m_pBuffer[m_cursor++ % m_filterSize] = value;
	}
	T GetSmoothed() const {
		UINT n = m_cursor < m_filterSize ? m_cursor : m_filterSize;

		if(!n) return m_stdVal;
		else
		{
			T avg;
			UINT i;
			ZeroMemory(&avg, sizeof(avg));

			for ( i = 0; i < n; i++) 
				avg += m_pBuffer[i];

			avg /= static_cast<T>(i);
			return avg;
		}
	}
	void Reset() { 
		m_cursor = 0;
	}
};

//////////////////////////////////////////////////////////////////////////
// ICGLGameLoop Event handler
__interface CGL_API ICGLGameLoopEventHandler
{
	void OnRender(double dTime, float fElapsed);
	bool OnUpdate(double dTime, float fElapsed);
	void OnIdle();
	void OnReset();
	void OnRestore();
	void OnDeviceLost();
	void OnWindowOccluded();
	void OnWindowShown();
};

//////////////////////////////////////////////////////////////////////////
// CGLGameLoop
class CGL_API CGLGameLoop : protected CGLManagerConnector
{
	ICGLGameLoopEventHandler* m_pEvtHandler;
	bool m_fullSpeed;
	float m_fixedFrameRate;
	Smoother<float>* m_pFrameSmoother;
	HWND m_window;

	bool m_running;
	bool m_quitting;

	double m_time;
	double m_timeSmoothed;
	float m_elapsed;
	float m_elapsedSmoothed;

	PCGLTimer m_loopTimer;
	PCGLTimer m_gpuTimer;
	PCGLTimer m_onUpdateTimer;
	PCGLTimer m_onIdleTimer;
	PCGLTimer m_onRenderTimer;

public:
	CGLGameLoop(ICGLGameLoopEventHandler* pHandler, HWND window = NULL, float updateInterval = 1.0f / 60.0f );
	~CGLGameLoop();
	void Run();

	void SetUpdateInterval(float updateInterval);
	inline float UpdateInterval()				{ return m_fixedFrameRate; }
	inline void EnableFullSpeed(bool enable)	{ m_fullSpeed = enable; }
	inline bool Running()						{ return m_running; }
	inline bool Quitting()						{ return m_quitting; }
	inline void Quit()							{ m_quitting = true; }

	inline float ElapsedTime()					{ return m_elapsedSmoothed; }
	inline float ElapsedTimeExact()				{ return m_elapsed; }
	inline double Time()						{ return m_timeSmoothed; }
	inline double TimeExact()					{ return m_time; }

	inline float DrawTime()						{ return m_gpuTimer->get(); }//m_drawTime; }
	inline float OnUpdateTime()					{ return m_onUpdateTimer->get(); }
	inline float OnIdleTime()					{ return m_onIdleTimer->get(); }
	inline float OnRenderTime()					{ return m_onRenderTimer->get(); }
};

}