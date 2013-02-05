#include "main.h"

int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	MEM_LEAK_TRACING();

 	cgl::core::PCGLManager pMgr = cgl::CGLCreate(true);

	// add log listener
	LogListener listener;
	listener.SetThresholdCategory(cgl::debug::CGL_LOG_CATEGORY_WARNING);

	{
		// create window
		cgl::util::PCGLWindow pWindow = cgl::util::CGLWindowFromConfig::Create(L"Test", 640, 480, DefWindowProc);
		CGLTraceObj(pWindow);
		if(!pWindow->restore())
			return 1;

		// create dxgi factory
		cgl::core::PDXGIFactory pFactory = cgl::core::CDXGIFactory::Create();
		CGLTraceObj(pFactory);

		// get primary hardware adapter
		cgl::core::PDXGIAdapter pAdapter= cgl::core::CDXGIAdapter::Create(pFactory, 0);
		CGLTraceObj(pAdapter);

		// get adapter outputs
		cgl::core::PDXGIOutput pOutput= cgl::core::CDXGIOutput::Create(pAdapter, 0);
		CGLTraceObj(pOutput);
		if(!pOutput->restore())
			return 4;

		// get output formats
		std::vector<DXGI_MODE_DESC> formats = pOutput->GetFormats();
		if(formats.empty())
			return 5;

		// create device
		cgl::core::PD3D11Device pDevice = cgl::core::CD3D11Device::Create(pAdapter, D3D11_CREATE_DEVICE_DEBUG);
		if(!pDevice)
			return 6;

		CGLTraceObj(pDevice);

		// create swap chain
		cgl::core::PDXGISwapChain pSwapChain = cgl::core::CDXGISwapChain::Create(pWindow, pOutput, cgl::core::CGLSwapChainDesc(formats.size()-1));
		CGLTraceObj(pSwapChain);
		if(!pSwapChain->restore())
			return 7;

		pMgr->Reset();
		if(!pMgr->Restore())
			return -1;

		while(IsWindowEnabled(pWindow->get()))
		{
			cgl::util::CGLWindow::PeekMessages(pWindow->get());
		}
	}

	return 0;
}

void LogListener::Process( const cgl::debug::CGLLogEntry& notification )
{
	std::string out;
	switch(notification.Category())
	{
	case cgl::debug::CGL_LOG_CATEGORY_ERROR:
		{ 
			out += "<font color=red>"	 + notification.Message();

			// check if its an cgl object related error
			cgl::core::CGLObject* pObject = notification.RelatedObj();
			if(pObject)
			{
				out += ":";
				out += "<br>";
				out += "&nbsp&nbsp&nbsp&nbsp ";
				out += DXGetErrorStringA(pObject->getLastResult());
				out += "<br>";
				out += "&nbsp&nbsp&nbsp&nbsp ";
				out += DXGetErrorDescriptionA(pObject->getLastResult());
			}

			out += "</font>"; 
			
		} break;

	case cgl::debug::CGL_LOG_CATEGORY_WARNING:	out += "<font color=orange>" + notification.Message() + "</font>"; break;
	case cgl::debug::CGL_LOG_CATEGORY_INFO:		out += "<font color=blue>"	 + notification.Message() + "</font>"; break;
	default: out += "<font color=black>" + notification.Message() + "</font>"; break;
	}

	if(notification.Category() > m_maxCat)
	{
		m_maxCat = notification.Category();
	}

	m_currBlock += std::string("<li>");
	m_currBlock += std::string(out.c_str());
	m_currBlock += std::string("</li>\n");
}

void LogListener::Process( const std::string& message )
{
	m_currBlock += std::string("<li>");
	m_currBlock += std::string(message.c_str());
	m_currBlock += std::string("</li>\n");

	if(m_blockFinished)
	{
		if(m_maxCat >= m_thresholdCat)
		{
			WriteCurrentBlock();
			m_maxCat = cgl::debug::CGL_LOG_CATEGORY_INFO;
		}

		m_currBlock.clear();
		m_blockFinished = false;
	}

	//_RPT0(0, ((std::string)message + std::string("\n")).c_str() );
}

void LogListener::Intend( int direction )
{
	std::string type;
	m_indent += direction;

	switch(m_indent % 3)
	{
	case 0: type = "type=\"square\""; break;
	case 1: type = "type=\"circle\""; break;
	case 2: type = "type=\"disc\""; break;
	}
	
	if(direction == 1)
	{
		m_currBlock += std::string("<ul ") + type + std::string(" >\n");
	}
	else
	{
		m_currBlock += std::string("</ul>\n");
	}

	if (m_indent == 0)
	{
		m_blockFinished = true;
	}
}

void LogListener::WriteCurrentBlock()
{
	if(m_currBlock.empty())
		return;

	m_pFile = fopen("cgl.html", "a");
	if (m_pFile)
	{
		fputs(m_currBlock.c_str(), m_pFile);
		fclose(m_pFile);
	}
}
