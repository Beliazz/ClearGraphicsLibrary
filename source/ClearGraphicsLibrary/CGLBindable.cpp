#include "cgl.h"

cgl::core::CGLBindable::CGLBindable() : m_upToDate__(false)
{

}

void cgl::core::CGLBindable::Bind()
{
	CGLAccess::CGLDevice()->Bind(this);
}

cgl::core::CGLBindable::~CGLBindable()
{
	for (auto it = m_params.begin(); it != m_params.end(); it++)
	{
		if (*it)
		{
			free(*it);
		}
	}
}

