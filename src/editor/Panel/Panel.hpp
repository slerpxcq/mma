#pragma once

namespace mm
{

class Panel
{
public:
	Panel(const String& name) : m_name{ name } {}
	virtual ~Panel() = default;
	virtual void OnUpdate(f32 deltaTime) {};
	virtual void OnRender();

protected:
	String m_name{};
	bool m_focused{};
	bool m_hovered{};
	bool m_opened{};
	bool m_resized{};
	Vec2u m_contentSize{};
	Vec2u m_windowSize{};
	Vec2u m_contentMax{};
	Vec2u m_contentMin{};
	Vec2u m_contentPos{};
};

}

