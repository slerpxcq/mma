#pragma once

namespace mm
{

class Panel
{
public:
	Panel(StringView name) : m_name{ name } {}
	virtual ~Panel() = default;
	virtual void OnUpdate(f32 deltaTime) = 0;
	virtual void OnRender() = 0;

protected:
	void OnBegin();

protected:
	const String m_name{};
	bool m_focused{};
	bool m_hovered{};
	bool m_opened{};
	bool m_resized{};
	Vec2 m_contentSize{};
	Vec2 m_windowSize{};
	Vec2 m_contentMax{};
	Vec2 m_contentMin{};
	Vec2 m_contentPos{};
};

}

