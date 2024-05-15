#pragma once

namespace mm
{
	class Entity;
	class Command;

	namespace EditorEvent
	{
		struct CommandIssued {
			Command* command;
		};

		struct EntitySelected {
			Entity* entity;
		};

		struct FrameSet {
			uint32_t frame;
		};
	}
}
