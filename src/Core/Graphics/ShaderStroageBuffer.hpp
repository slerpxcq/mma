#pragma once

#include "Buffer.hpp"

namespace mm
{

class ShaderStroageBuffer : public Buffer
{
public:
	ShaderStroageBuffer() : 
		Buffer{ Target::SHADER_STORAGE } {}

	using Buffer::SetStorage;
	using Buffer::SetSubData;
	using Buffer::SetBindBase;

private:
};

}

