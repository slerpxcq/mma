#pragma once

#include "Buffer.hpp"

namespace mm
{

class BindableBuffer : public Buffer
{
public:
	BindableBuffer(uint32_t target) : Buffer(target) {
		MM_ASSERT((target == GL_UNIFORM_BUFFER || 
				   target == GL_SHADER_STORAGE_BUFFER) && "Invalid buffer target!");
	}

	void BindBase(uint32_t base) const { glBindBufferBase(m_target, base, m_rendererID); }
};

}

