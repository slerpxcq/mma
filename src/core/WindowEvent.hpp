#pragma once

#include <cstdint>

namespace mm
{

namespace WindowEvent
{
struct WindowSized {
	u32 x, y;
};

struct WindowClosed {};

struct KeyPressed {
	u32 code;
	u32 mods;
	bool repeat;
};

struct KeyReleased {
	u32 code;
};

struct KeyTyped {
	u32 code;
};

struct MouseButtonPressed {
	u32 button;
};

struct MouseButtonReleased {
	u32 button;
};

struct MouseMoved {
	f32 x, y;
};

struct MouseScrolled {
	f32 delta;
};
};

}


