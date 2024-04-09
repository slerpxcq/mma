#pragma once

namespace mm
{
	class Sequencer
	{
	public:
		struct Item {
			std::string name;
			int32_t type;
			int32_t index;
		};

		struct Group {
			std::string name;
			bool expanded = false;
			std::vector<Item> items;
		};

	public:
		void OnUIRender();
		void AddGroup(const Group& group) { m_groups.push_back(group); }

	private:
		std::vector<Group> m_groups;
	};
}

