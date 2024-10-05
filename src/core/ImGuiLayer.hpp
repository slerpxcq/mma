#pragma once

#include "Layer/Layer.hpp"

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

	void OnUpdate(float deltaTime) override;
	void OnRender() override;

	void Begin();
	void End();

	ImFont* GetFont() const { return m_font; }

private:
	std::unique_ptr<Impl> m_impl{};
	ImFont* m_font{};
};

}

