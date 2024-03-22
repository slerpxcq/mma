#pragma once

namespace mm
{
	class Renderer
	{
	public:
		using Command = std::function<void()>;

	public:
		Renderer();
		void Submit(Command cmd);
		void Commit();

	private:
		std::vector<Command> m_cmdQueue;
	};
}

