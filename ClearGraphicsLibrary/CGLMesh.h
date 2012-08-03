#ifndef CGLMesh_h__
#define CGLMesh_h__

#include "cgl.h"

namespace cgl
{
	
//////////////////////////////////////////////////////////////////////////
// cgl mesh base class
class CGL_API CGLMesh : public CGLObject
{
private:
	std::vector<PCGLEffectPass> m_pPasses;
	std::vector<PD3D11VertexBuffer> m_pVertexBuffer;

	CGLMesh(PD3D11EffectTechnique pTechnique);

public:

};

}

#endif // CGLMesh_h__