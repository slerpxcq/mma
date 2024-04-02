#pragma once

namespace mm
{
	class Input
	{
	public:
		static glm::vec2 MousePos();
		static bool MouseButtonPressed(int32_t button);
		static bool KeyPressed(int32_t key);
	};
}

