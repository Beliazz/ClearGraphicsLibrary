#include "cgl.h"

cgl::CGLGameLoop::CGLGameLoop( ICGLGameLoopEventHandler* pHandler, HWND window, float updateInterval /*= 1.0f / 60.0f */ ) :
	m_pEvtHandler(pHandler), m_fixedFrameRate(updateInterval)
{
	m_window = window;
	m_fullSpeed = false;
	m_pFrameSmoother = new Smoother<float>(5, updateInterval);
	m_quitting = false;
	m_running = false;
	m_time = 0.0;
	m_timeSmoothed = 0.0;
	m_elapsed = 0.0f;
	m_elapsedSmoothed = 0.0f;

// 	D3D11_QUERY_DESC timeQueryDesc;
// 	ZeroMemory(&timeQueryDesc, sizeof(timeQueryDesc));
// 	timeQueryDesc.Query = D3D11_QUERY_TIMESTAMP;
// 	m_timeQuery = cgl::CD3D11Query::Create(timeQueryDesc);
// 	m_timeQuery->restore();
// 
// 	D3D11_QUERY_DESC disjointQueryDesc;
// 	ZeroMemory(&disjointQueryDesc, sizeof(disjointQueryDesc));
// 	disjointQueryDesc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
// 	m_disjointQuery = cgl::CD3D11Query::Create(disjointQueryDesc);
// 	m_disjointQuery->restore();

	m_gpuTimer = CGLGpuTimer::Create();
	m_onUpdateTimer = CGLCpuTimer::Create();
	m_loopTimer = CGLCpuTimer::Create();
	m_onIdleTimer = CGLCpuTimer::Create();
	m_onRenderTimer = CGLCpuTimer::Create();

	m_measureDrawTime = false;
}

void cgl::CGLGameLoop::Run()
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

			if(m_measureDrawTime)
				m_gpuTimer->Stop();

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
			
			// remove chunk
			mgr()->Tidy();

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

		// idle 
		// 
		// process window messages
		m_onIdleTimer->Start();
		m_pEvtHandler->OnIdle();
		m_onIdleTimer->Stop();

		// measure end ticks
		m_loopTimer->Stop();

		m_time += m_loopTimer->get();
		m_pFrameSmoother->Add(m_loopTimer->get());

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

