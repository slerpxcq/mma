#pragma once

#include <cstdint>

namespace mm
{
	class Node;
	class Command;

	namespace EditorEvent
	{
		struct CommandIssued {
			Command* command;
		};

		struct NodeSelected {
			Node* node;
		};

		struct FrameSet {
			uint32_t frame;
		};
	}
}
