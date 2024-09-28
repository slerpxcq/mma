#pragma once

#include <cstdint>

namespace mm
{

namespace WindowEvent
{
struct WindowSized {
	uint32_t x, y;
};

struct WindowClosed {};

struct KeyPressed {
	uint32_t code;
	uint32_t mods;
	bool repeat;
};

struct KeyReleased {
	uint32_t code;
};

struct KeyTyped {
	uint32_t code;
};

struct MouseButtonPressed {
	uint32_t button;
};

struct MouseButtonReleased {
	uint32_t button;
};

struct MouseMoved {
	float x, y;
};

struct MouseScrolled {
	float delta;
};
};

}


