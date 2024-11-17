#pragma once

#include <imgui.h>

namespace mm
{

class KeyMap
{
public:
	void Insert(StringView name, ImGuiKey key) { m_map.insert(MakePair(name, key)); }
	ImGuiKey& operator[](StringView name) { return m_map.at(String{ name }); }

private:
	HashMap<String, ImGuiKey> m_map;
};

}

