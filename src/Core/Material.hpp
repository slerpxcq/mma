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
	enum Flags {
		NONE = 0,
		NO_CULL_BIT = (1 << 0),
	};

public:
	Material(StringView name, 
			 InitList<Pair<MapType, Ref<Texture>>> maps,
			 Ref<Program> program,
			 u32 flags = 0) :
		Resource{ name },
		m_program{ program },
		m_maps{ maps.begin(), maps.end() } {
	}

	u32 GetFlags() const { return m_flags; }
	const Program& GetProgram() const { return *m_program; }
	const Texture& GetMap(MapType type) const { return *(m_maps.find(type))->second; }

private:
	u32 m_flags{};
	HashMap<MapType, Ref<Texture>> m_maps{};
	Ref<Program> m_program{};
};

}

