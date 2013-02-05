#pragma once

#include "cgl.h"

namespace cgl 
{
	namespace time
	{
		//////////////////////////////////////////////////////////////////////////
		// ICGLGameLoop Event handler
		__interface CGL_API ICGLGameLoopEventHandler
		{
			void OnRender(double dTime, float fElapsed);
			void OnUpdate(double dTime, float fElapsed);
			HRESULT OnPresent(bool test = false);
			void OnPostUpdate(double dTime);
			void OnIdle();
			void OnReset();
			void OnRestore();
			void OnDeviceLost();
			void OnWindowOccluded();
			void OnWindowShown();
		};

		//////////////////////////////////////////////////////////////////////////
		// CGLGameLoop
		class CGL_API CGLGameLoop : protected cgl::core::CGLAccess
		{
			ICGLGameLoopEventHandler* m_pEvtHandler;
			bool m_fullSpeed;
			float m_fixedFrameRate;
			CGLSmoother<float>* m_pFrameSmoother;
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

			bool m_measureDrawTime;

		public:
			CGLGameLoop(ICGLGameLoopEventHandler* pHandler, HWND window = NULL, float updateInterval = 1.0f / 60.0f );
			~CGLGameLoop();
			void Run();

			void SetUpdateInterval(float updateInterval);
			void EnableDrawTimeMeasurement(bool enable);

			inline float UpdateInterval()				{ return m_fixedFrameRate; }
			inline void EnableFullSpeed(bool enable)	{ m_fullSpeed = enable; }
			inline bool Running()						{ return m_running; }
			inline bool Quitting()						{ return m_quitting; }
			inline void Quit()							{ m_quitting = true; }

			inline float ElapsedTime()					{ return m_elapsedSmoothed; }
			inline float ElapsedTimeExact()				{ return m_elapsed; }
			inline double Time()						{ return m_timeSmoothed; }
			inline double TimeExact()					{ return m_time; }

			inline float FPSExact()						{ return 1.0f / m_elapsed;}
			inline float FPS()							{ return 1.0f / m_elapsedSmoothed;}

			inline float DrawTime()						{ return m_gpuTimer->get(); }
			inline float OnUpdateTime()					{ return m_onUpdateTimer->get(); }
			inline float OnIdleTime()					{ return m_onIdleTimer->get(); }
			inline float OnRenderTime()					{ return m_onRenderTimer->get(); }
		};
	}
}