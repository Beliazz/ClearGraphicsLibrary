#pragma once

#include "cgl.h"

namespace cgl 
{
	namespace time
	{
		//////////////////////////////////////////////////////////////////////////
		// Smoother
		template<typename T> 
		class CGL_API CGLSmoother
		{
		private:
			T*		m_pBuffer;
			T		m_stdVal;
			UINT	m_filterSize;
			UINT	m_cursor;
		public:
			CGLSmoother(int filterSize, const T& stdVal) 
				:  m_cursor(0), m_filterSize(filterSize), m_stdVal(stdVal)
			{
				m_pBuffer = new T[m_filterSize];
			}
			~CGLSmoother(void) { SAFE_DELETE_ARRAY(m_pBuffer);}

			void Add(const T& value) {
				m_pBuffer[m_cursor++ % m_filterSize] = value;
			}
			T GetSmoothed() const {
				UINT n = m_cursor < m_filterSize ? m_cursor : m_filterSize;

				if(!n) return m_stdVal;
				else
				{
					T avg;
					UINT i;
					ZeroMemory(&avg, sizeof(avg));

					for ( i = 0; i < n; i++) 
						avg += m_pBuffer[i];

					avg /= static_cast<T>(i);
					return avg;
				}
			}
			void Reset() { 
				m_cursor = 0;
			}
		};
	}
}