#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "Debug.h"
#include <cgl.h>
#pragma comment(lib, "ClearGraphicsLibrary.lib")

class LogListener : public cgl::debug::CGLLogListener
{
private:
	FILE* m_pFile;
	int m_indent;
	bool m_blockFinished;
	std::string m_currBlock;
	UINT m_maxCat;
	UINT m_thresholdCat;

	virtual void Process( const cgl::debug::CGLLogEntry& notification );
	virtual void Process( const std::string& message );
	virtual void Intend( int direction );
	virtual void WriteCurrentBlock();

public:
	LogListener() : m_pFile(NULL), m_indent(0) 
	{
		cgl::debug::CGLLog::AddListener(this);
		m_pFile = fopen("cgl.html", "w");
		if (m_pFile)
		{
			fputs("<body font-family=\"consolas\">", m_pFile);
			fputs("<ul>", m_pFile);
			fclose(m_pFile);
		}

		m_currBlock.clear();
		m_thresholdCat = cgl::debug::CGL_LOG_CATEGORY::CGL_LOG_CATEGORY_INFO;
		m_maxCat = cgl::debug::CGL_LOG_CATEGORY::CGL_LOG_CATEGORY_INFO;
		m_blockFinished = false;
	}
	~LogListener()
	{
		WriteCurrentBlock();
		m_pFile = fopen("cgl.html", "a");
		if (m_pFile)
		{
			for (int i = 0; i < m_indent; i++)
			{
				fputs("</ul>", m_pFile);
			}

			fputs("</body>", m_pFile);
			fclose(m_pFile);
		}

		cgl::debug::CGLLog::RemoveListener(this);
	}

	void SetThresholdCategory(cgl::debug::CGL_LOG_CATEGORY cat)
	{
		m_thresholdCat = cat;
	}
};