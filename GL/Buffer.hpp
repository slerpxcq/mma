#pragma once

#include "GPUResource.hpp"

namespace mm
{

class Buffer : public GPUResource
{
public:
	Buffer(uint32_t target);
	virtual ~Buffer();

	void Data(uint32_t size, void* data, uint32_t usage = GL_STATIC_DRAW);
	void SubData(uint32_t offset, uint32_t size, void* data);
	
protected:
	uint32_t m_size = 0;
	uint32_t m_target = 0;
};

}

