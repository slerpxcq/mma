#include "CorePch.hpp"
#include "Graphics.hpp"

namespace mm
{

Graphics::Graphics(Config::API api) :
	m_api{ api }
{
}

u32 Graphics::ToSize(IndexType type) {
	switch (type) {
	case IndexType::UBYTE:
		return 1;
	case IndexType::USHORT:
		return 2;
	case IndexType::UINT:
		return 4;
	default:
		MM_CORE_UNREACHABLE();
	}
}

Graphics::IndexType Graphics::ToIndexType(u32 size) {
	switch (size) {
	case 1:
		return IndexType::UBYTE;
	case 2:
		return IndexType::USHORT;
	case 4:
		return IndexType::UINT;
	default:
		MM_CORE_UNREACHABLE();
	}
}

}
