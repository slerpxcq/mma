#pragma once

#include "Drawable.hpp"
#include "Graphics/VertexArray.hpp"

namespace mm
{

class Mesh : public Drawable
{
public:
	Mesh(const String& name, 
		 Ref<VertexArray> va, 
		 u32 indexCount, u32 indexOffset = 0) :
		m_name{ name },
		m_indexOffset{ indexOffset },
		m_indexCount{ indexCount },
		m_vertexArray{ va } {}

	u32 GetIndexCount() const { return m_indexCount; }

private:
	String m_name{};
	Ref<VertexArray> m_vertexArray{};
	u32 m_indexOffset{};
	u32 m_indexCount{};
};

}

