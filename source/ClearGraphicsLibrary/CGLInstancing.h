#pragma once

#include "cgl.h"

namespace cgl {

//////////////////////////////////////////////////////////////////////////
// typedef
__interface ICGLInstancing;
typedef std::tr1::shared_ptr<ICGLInstancing> PCGLInstancing;

//////////////////////////////////////////////////////////////////////////
// instancing 
// 
// decides if an element data is per vertex or per instance 
__interface CGL_API ICGLInstancing
{
	void getElementInstancingInfo(LPCSTR semanticName, UINT& semanticIndex, UINT* __out dataStep, UINT* __out instanceBufferSlot);
};

}
