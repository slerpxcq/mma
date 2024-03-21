#pragma once

#include <cstdint>

namespace mm
{
	struct Event {
	public:
		enum class Source {
			WINDOW = 1 << 0,
			KEYBOARD = 1 << 1,
			MOUSE = 1 << 2,
			MOUSE_BUTTON = 1 << 3
		};

		enum class Type {
			NONE = 0,
			WINDOW_RESIZED, WINDOW_CLOSED,
			KEY_PRESSED, KEY_RELEASED, KEY_TYPED,
			MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED,
			MOUSE_MOVED_, MOUSE_SCROLLED
		};

		struct WindowResize {
			uint32_t w, h;
		};

		struct Key {
			uint32_t code;
			bool repeat;
		};

		struct MouseButton {
			uint32_t code;
		};

		struct MouseMove {
			float x, y;
		};

		struct MouseScroll {
			float delta;
		};

	public:
		Source source;
		Type type;
		bool handled;
		union {
			WindowResize windowResize;
			Key key;
			MouseButton mouseButton;
			MouseMove mouseMove;
			MouseScroll mouseScroll;
		} data;
	};
}


