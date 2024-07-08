#pragma once

struct GLFWwindow;

namespace mm
{

class Application 
{
public:
	struct Info {
		std::string title = "";
		uint32_t width = 1280;
		uint32_t height = 720;
	};

public:
	virtual void Run();
	virtual ~Application() {}
	virtual void Startup();
	virtual void Shutdown();

protected:
	virtual void NewFrame(float deltaTime) {}

protected:
	GLFWwindow* m_window = nullptr;
	Info m_info;
	bool m_running = true;
	bool m_minimized = false;
};

}


