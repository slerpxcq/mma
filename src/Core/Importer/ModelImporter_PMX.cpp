#include "CorePch.hpp"
#include "ModelImporter.hpp"

#include "../File/PMXFile.hpp"

#include <yaml-cpp/yaml.h>

namespace mm
{

Path ModelImporter::ImportPMX(const Path& path)
{
	PMXFile pmx{ path };
	return Path{};
}

}
