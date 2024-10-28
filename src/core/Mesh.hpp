#pragma once

#include "Drawable.hpp"

namespace mm
{

class Mesh : public Drawable
{
public:
private:
	u32 m_vertexCount;
	u32 m_indexCount;
	Ref<Buffer> m_vertexBuffer{};
	Ref<Buffer> m_indexBuffer{};
};

}

