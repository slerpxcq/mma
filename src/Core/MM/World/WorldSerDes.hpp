#pragma once

#include "World.hpp"

namespace mm
{
	class WorldSerDes
	{
	public:
		static void SerializeWorld(const World& world, const std::filesystem::path& path);
		static std::unique_ptr<World> DeserializeWorld(const std::filesystem::path& path);

	private:
	};
}

