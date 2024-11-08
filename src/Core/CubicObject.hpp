#pragma once

namespace mm
{

class CubicObject
{
public:
	virtual ~CubicObject() = default;
	CubicObject(u32 width, u32 height, u32 depth) :
		m_width{width}, m_height{height}, m_depth{depth} {}

	u32 GetWidth() const { return m_width; }
	u32 GetHeight() const { return m_height; }
	u32 GetDepth() const { return m_depth; }
	Vec3 GetSize() const { return { m_width, m_height, m_depth }; }

protected:
	u32 m_width{};
	u32 m_height{};
	u32 m_depth{};
};

}

