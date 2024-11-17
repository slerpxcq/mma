#pragma once

namespace mm
{

class DebugDraw
{
public:
	MM_NONMOVABLE(DebugDraw)
	virtual ~DebugDraw() = default;
	DebugDraw() = default;
	virtual void DrawLine(const Vec3& from, const Vec3& to, const Color& color) const = 0;

	void SetLineSize(f32 size) { m_lineSize = size; }

protected:
	f32 m_lineSize{ 1.f };
};

}

