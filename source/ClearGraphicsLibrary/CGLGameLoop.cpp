#include "cgl.h"

cgl::CGLGameLoop::CGLGameLoop( ICGLGameLoopEventHandler* pHandler, float updateInterval /*= 1.0f / 60.0f */ ) :
	m_pEvtHandler(pHandler), m_fixedFrameRate(updateInterval)
{
	m_fullSpeed = false;
	m_pFrameSmoother = new Smoother<float>(10, updateInterval);
	m_quitting = false;
	m_running = false;
	m_time = 0.0;
	m_timeSmoothed = 0.0;
	m_elapsed = 0.0f;
	m_elapsedSmoothed = 0.0f;
}

void cgl::CGLGameLoop::Run()
{
	UINT64 start;
	UINT64 end;
	UINT64 frequency;

	bool updated = false;
	bool drawn = false;
	bool occluded = false;

	MSG msg;

	// get timer frequency
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

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
		QueryPerformanceCounter((LARGE_INTEGER*)&start);

		// if the game was updated
		// and the window is visible
		// render
		if(updated && !occluded)
		{
			m_pEvtHandler->OnRender(m_timeSmoothed, updateInterval);

			drawn = true;
			updated = false;
		}

		// if the elapsed time is greater than
		// the screen refresh rate update it
		if (timeAccount >= updateInterval)
		{
			while (timeAccount >= updateInterval)
			{
				m_pEvtHandler->OnUpdate(m_timeSmoothed, updateInterval);
				timeAccount -= updateInterval;
			}
			
// 			// remove chunk
// 			mgr()->Tidy();

			// if the window was not visible previously
			// perform a check if its still occluded
			if (occluded)
			{
				HRESULT hResult = mgr()->GetDevice()->GetSwapChain()->Present(0, DXGI_PRESENT_TEST);
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

			updated = true;
		}

		// if the game was drawn
		// present it
		if (drawn)
		{
			HRESULT hResult = mgr()->GetDevice()->GetSwapChain()->Present(0, 0);
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

		// process window messages
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// measure end ticks
		QueryPerformanceCounter((LARGE_INTEGER*)&end);

		// calculate elapsed time in seconds and
		// add it to the frame smoother
		m_elapsed =  (float)(end - start) / frequency;
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

void cgl::CGLGameLoop::SetUpdateInterval( float updateInterval )
{
	m_fixedFrameRate = updateInterval;
	m_pFrameSmoother = new Smoother<float>(5, updateInterval);
}

cgl::CGLGameLoop::~CGLGameLoop()
{
	SAFE_DELETE(m_pFrameSmoother);
}

