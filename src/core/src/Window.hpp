#pragma once

namespace mm
{

class Window
{
public:
	struct ConstructInfo {
		uint32_t width{ 1280 };
		uint32_t height{ 720 };
		GraphicsAPI api{ GraphicsAPI::GL4 };
		std::string_view title{ "" };
	};

	class Impl
	{
	public:
		Impl(const ConstructInfo& info) : 
			m_title{info.title},
			m_width{info.width},
			m_height{info.height} {}

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void* GetHandle() const = 0;
		virtual ~Impl() {}

	private:
		std::string_view m_title{ "" };
		uint32_t m_width{};
		uint32_t m_height{};
	};

public:
	Window(const ConstructInfo& info = ConstructInfo());
	GraphicsAPI GetAPI() const { return m_api; }
	void* GetHandle() const { return m_impl->GetHandle(); }
	void BeginFrame() { m_impl->BeginFrame(); }
	void EndFrame() { m_impl->EndFrame(); }

private:
	GraphicsAPI m_api{};
	std::unique_ptr<Impl> m_impl{};
};

}

