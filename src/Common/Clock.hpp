#pragma once

#include <chrono>

namespace mm
{

using TimePoint = decltype(std::chrono::high_resolution_clock::now());

namespace Clock
{

static inline TimePoint Now() 
{ 
	return std::chrono::high_resolution_clock::now(); 
}

static inline float Delta(TimePoint x) 
{ 
	return (std::chrono::duration_cast<std::chrono::microseconds>(Now() - x).count() * 1e-6f); 
}

}

}
