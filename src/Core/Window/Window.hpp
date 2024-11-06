#pragma once

namespace mm
{

class Window
{
public:
	struct ConstructInfo {
		u32 width{ 1280 };
		u32 height{ 720 };
		StringView title{ "" };
	};

	class Impl {
	public:
		virtual ~Impl() = default;
		Impl(const Window::ConstructInfo& info) : 
			m_title{info.title},
			m_width{info.width},
			m_height{info.height} {}

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void* GetHandle() const = 0;

	private:
		StringView m_title{""};
		u32 m_width{};
		u32 m_height{};
	};

public:
	Window(const ConstructInfo& info = {});
	void* GetHandle() const { return m_impl->GetHandle(); }
	void BeginFrame() { m_impl->BeginFrame(); }
	void EndFrame() { m_impl->EndFrame(); }

private:
	Scoped<Impl> m_impl{};
};

}

