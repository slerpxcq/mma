#pragma once

#include "Resource.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Program.hpp"

namespace mm
{

class Material : public Resource
{
public:
	enum class MapType { ALBEDO };

public:
	Material(StringView name, 
			 InitList<Pair<MapType, Ref<Texture>>> maps,
			 Ref<Program> program) :
		Resource{ name },
		m_program{ program } {
		for (auto& map : maps) {
			m_maps.insert(map);
		}
	}

private:
	HashMap<MapType, Ref<Texture>> m_maps{};
	Ref<Program> m_program{};
};

}

