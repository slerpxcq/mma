#pragma once

namespace mm
{
	class GLRenderer
	{
	public:
		using Command = std::function<void()>;

	public:
		GLRenderer();
		void Submit(Command cmd);
		void Commit();

	private:
		std::vector<Command> m_cmdQueue;
	};
}

