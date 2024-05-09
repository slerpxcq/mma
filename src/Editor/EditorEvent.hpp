#pragma once

//#include "Core/MM/Math/Transform.hpp" 

#include "Commands.hpp"

namespace mm
{
	class Entity;

	namespace EditorEvent
	{
		struct CommandIssued {
			Command* command;
		};

		struct ItemSelected {
			uint8_t type;
			uint32_t index;
			std::any item;
		};

		struct FrameSet {
			uint32_t frame;
		};
	}
}
