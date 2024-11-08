#pragma once

namespace mm
{

class RectObject
{
public:
	virtual ~RectObject() = default;
	RectObject(u32 width, u32 height) :
		m_width{width}, m_height{height} {}

	u32 GetWidth() const { return m_width; }
	u32 GetHeight() const { return m_height; }
	Vec2 GetSize() const { return { m_width, m_height }; }

protected:
	u32 m_width{};
	u32 m_height{};
};

}

