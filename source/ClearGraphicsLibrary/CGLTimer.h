#pragma once

#include "cgl.h"

namespace cgl {

class CGLTimer;
typedef std::shared_ptr<CGLTimer> PCGLTimer;

class CGL_API CGLTimer
{
private:
	UINT64 m_start;
	UINT64 m_end;
	UINT64 m_frequency;
	float m_elapsed;

	CGLTimer();

public:
	static PCGLTimer Create();

	void Start();
	float Stop();
	float get();
};


}