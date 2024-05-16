#pragma once

#include "mmpch.hpp"

namespace mm
{
	class EdgeDetector
	{
	public:
		/* Return value:*/
		/* 0: hold; 1: rising; -1: falling */
		int32_t Update(int32_t value) {
			int32_t ret = value - m_value;
			m_value = value;
			return ret;
		}

	private:
		int32_t m_value;
	};
}
