#pragma once

#include "Buffer.hpp"

namespace mm
{

class IndexBuffer : public Buffer
{
public:
	IndexBuffer() : Buffer{ Target::INDEX } {}

private:
	u32 m_indexCount{};
	u32 m_indexSize{};
};

}

