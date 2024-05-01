#include "mmpch.hpp"
#include "WorldSerDes.hpp"

#include <yaml-cpp/yaml.h>

namespace mm
{
	void WorldSerDes::SerializeWorld(const World& world, const std::filesystem::path& path)
	{
	}

	std::unique_ptr<World> WorldSerDes::DeserializeWorld(const std::filesystem::path& path)
	{
		return std::unique_ptr<World>();
	}
}

