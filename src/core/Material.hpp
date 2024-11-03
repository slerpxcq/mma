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
	enum class Flags : u32 {
		NONE = 0,
		NO_CULL_BIT = (1 << 0),
	};

public:
	Material(StringView name, 
			 InitList<Pair<MapType, Ref<Texture>>> maps,
			 Ref<Program> program,
			 u32 flags = 0) :
		Resource{ name },
		m_program{ program } {
		for (auto& map : maps) {
			m_maps.insert(map);
		}
	}

	u32 GetFlags() const { return m_flags; }

private:
	u32 m_flags{};
	HashMap<MapType, Ref<Texture>> m_maps{};
	Ref<Program> m_program{};
};

}

