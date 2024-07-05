#pragma once

#include "Buffer.hpp"

namespace mm
{

class VertexBuffer : public Buffer
{
public:
	VertexBuffer() : Buffer(GL_ARRAY_BUFFER) {}
};

}

