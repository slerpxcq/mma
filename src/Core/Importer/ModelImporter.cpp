#include "CorePch.hpp"
#include "ModelImporter.hpp"

namespace mm
{

Path ModelImporter::Import(const Path& path)
{
	String extension = path.extension().string();
	if (extension == ".pmx") {
		return ImportPMX(path);
	}
	if (extension == ".x") {
		MM_CORE_UNINPLEMENTED();
	}
	if (extension == ".obj") {
		MM_CORE_UNINPLEMENTED();
	}
	else {
		throw RuntimeError("Unsupported model type");
	}
	return Path();
}

}
