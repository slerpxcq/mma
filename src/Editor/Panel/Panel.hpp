#pragma once

#include "Core/NamedObject.hpp"
#include "PanelOverlay.hpp"

namespace mm
{

class Panel : public NamedObject
{
public:
	Panel(StringView name) :
		NamedObject{ name } {}
	Panel(const Panel&) = delete;
	Panel(Panel&&) = default;
	virtual ~Panel() = default;
	virtual void OnUpdate(f32 deltaTime);
	virtual void OnRender() = 0;

	template <typename T, typename... Args> T* PushOverlay(Args&&... args);

	bool IsFocused() const { return m_focused; }
	bool IsHovered() const { return m_hovered; }
	bool IsOpened() const { return m_hovered; }
	bool IsResized() const { return m_resized; }
	Vec2 GetContentSize() const { return m_contentSize; }
	Vec2 GetWindowSize() const { return m_windowSize; }
	Vec2 GetWindowPos() const { return m_windowPos; }
	Vec2 GetContentMax() const { return m_contentMax; }
	Vec2 GetContentMin() const { return m_contentMin; }
	Vec2 GetContentPos() const { return m_contentPos; }

protected:
	void OnBegin();
	void OnEnd();

protected:
	bool m_focused{};
	bool m_hovered{};
	bool m_opened{};
	bool m_resized{};
	Vec2 m_contentSize{};
	Vec2 m_windowSize{};
	Vec2 m_windowPos{};
	Vec2 m_contentMax{};
	Vec2 m_contentMin{};
	Vec2 m_contentPos{};

	DynArray<Scoped<PanelOverlay>> m_overlays;
};

template <typename T, typename... Args>
T* Panel::PushOverlay(Args&&... args) 
{
	m_overlays.push_back(MakeScoped<T>(*this, std::forward<Args>(args)...));
	return static_cast<T*>(m_overlays.back().get());
}

}

