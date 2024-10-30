#pragma once

#include "Layer.hpp"

struct ImFont;

namespace mm
{

class Window;

class ImGuiLayer : public Layer
{
	static constexpr uint32_t FONT_SIZE = 20;
public:
	class Impl {
	public:
		virtual	~Impl() {}
		virtual void Begin() = 0;
		virtual void End() = 0;
	};

public:
	ImGuiLayer(const Window& window);

	virtual void OnUpdate(float deltaTime) override = 0;
	virtual void OnRender() override = 0;

	void Begin() const;
	void End() const;

	ImFont* GetFont() const { return m_font; }

	virtual ~ImGuiLayer() = default;

private:
	Scoped<Impl> m_impl{};
	ImFont* m_font{};
};

}

