#include "cgl.h"

cgl::time::CGLGameLoop::CGLGameLoop( ICGLGameLoopEventHandler* pHandler, HWND window, float updateInterval /*= 1.0f / 60.0f */ ) :
	m_pEvtHandler(pHandler), m_fixedFrameRate(updateInterval)
{
	m_window = window;
	m_fullSpeed = false;
	m_pFrameSmoother = new CGLSmoother<float>(10, updateInterval);
	m_quitting = false;
	m_running = false;
	m_time = 0.0;
	m_timeSmoothed = 0.0;
	m_elapsed = 0.0f;
	m_elapsedSmoothed = 0.0f;

	m_gpuTimer = CGLGpuTimer::Create("CGLGameLoop::DrawTime");
	m_onUpdateTimer = CGLCpuTimer::Create("CGLGameLoop::OnUpdate");
	m_loopTimer = CGLCpuTimer::Create("CGLGameLoop::Loop");
	m_onIdleTimer = CGLCpuTimer::Create("CGLGameLoop::OnIdle");
	m_onRenderTimer = CGLCpuTimer::Create("CGLGameLoop::OnRender");

	m_measureDrawTime = true;
}

void cgl::time::CGLGameLoop::Run()
{
	bool updated = false;
	bool drawn = false;
	bool occluded = false;

	// set running
	m_running = true;
	m_quitting = false;

	// reset time
	m_time = 0.0;
	m_timeSmoothed = 0.0;

	float updateInterval = m_fixedFrameRate;
	float timeAccount = updateInterval;

	while (!m_quitting)
	{
		// measure start ticks
		m_loopTimer->Start();

		// if the game was updated
		// and the window is visible
		// render
		if(updated && !occluded)
		{
			// render
			if(m_measureDrawTime)
				m_gpuTimer->Start();

			m_onRenderTimer->Start();
			m_pEvtHandler->OnRender(m_timeSmoothed, updateInterval);
			m_onRenderTimer->Stop();

			drawn = true;
			updated = false;
		}

		// if the elapsed time is greater than
		// the screen refresh rate update it
		if (timeAccount >= updateInterval)
		{
			while (timeAccount >= updateInterval)
			{
				m_onUpdateTimer->Start();
				m_pEvtHandler->OnUpdate(m_timeSmoothed, updateInterval);
				m_onUpdateTimer->Stop();

				timeAccount -= updateInterval;
			}
			
			// post update
			m_pEvtHandler->OnPostUpdate(m_timeSmoothed);

			// remove chunk
			// mgr()->Tidy();


			updated = true;
		}

		// if the window was not visible previously
		// perform a check if its still occluded
		if (occluded)
		{
			HRESULT hResult = m_pEvtHandler->OnPresent(true);

			switch ( hResult )
			{
				// the device has been removed
				// recreation of d3d11 objects needed
			case DXGI_ERROR_DEVICE_REMOVED:
			case DXGI_ERROR_DEVICE_RESET:
				{
					m_pEvtHandler->OnReset();
					m_pEvtHandler->OnDeviceLost();
				} break;

			case S_OK:
				{
					// the window is visible again 
					// -> resume rendering
					m_pEvtHandler->OnRestore();
					m_pEvtHandler->OnWindowShown();
					occluded = false;
				} break;
			}
		}

		// if the game was drawn
		// present it
		if (drawn)
		{
			HRESULT hResult = m_pEvtHandler->OnPresent();

			if(m_measureDrawTime)
				m_gpuTimer->Stop();

			switch ( hResult )
			{
				// the device has been removed
				// recreation of d3d11 objects needed
			case DXGI_ERROR_DEVICE_REMOVED:
			case DXGI_ERROR_DEVICE_RESET:
				{
					m_pEvtHandler->OnReset();
					m_pEvtHandler->OnDeviceLost();
				} break;

				// the window is occluded, do not render the game
				// until it is visible again
			case DXGI_STATUS_OCCLUDED:
				{
					m_pEvtHandler->OnReset();
					m_pEvtHandler->OnWindowOccluded();
					occluded = true;
				} break;
			}

			drawn = false;
		}

		// idle 
		// 
		// process window messages
		m_onIdleTimer->Start();
		m_pEvtHandler->OnIdle();
		m_onIdleTimer->Stop();

		// measure end ticks
		m_loopTimer->Stop();

		m_elapsed = m_loopTimer->get();
		m_time += m_elapsed;
		m_pFrameSmoother->Add(m_elapsed);

		// smoothed elapsed time
		m_elapsedSmoothed = m_pFrameSmoother->GetSmoothed();
		m_timeSmoothed += m_elapsedSmoothed;	

		if (m_fullSpeed)
		{
			updateInterval = m_elapsedSmoothed;
			timeAccount = m_elapsedSmoothed;
		}
		else
		{
			updateInterval = m_fixedFrameRate;
			timeAccount += m_elapsedSmoothed;	
		}
	}
}

void cgl::time::CGLGameLoop::SetUpdateInterval( float updateInterval )
{
	m_fixedFrameRate = updateInterval;
	m_pFrameSmoother = new CGLSmoother<float>(10, updateInterval);
}

cgl::time::CGLGameLoop::~CGLGameLoop()
{
	SAFE_DELETE(m_pFrameSmoother);
}

void cgl::time::CGLGameLoop::EnableDrawTimeMeasurement( bool enable )
{
	if (enable != m_measureDrawTime)
	{
		m_measureDrawTime = enable;	
		if (!m_measureDrawTime)
		{
			m_gpuTimer.reset();
		}
		else
		{
			m_gpuTimer = CGLGpuTimer::Create("CGLGameLoop::DrawTime");
		}	
	}
}

