#pragma once

namespace mm
{

/* ModelImporter:
 * Converts model to gltf
 * 
 */
class ModelImporter
{
public:
	static Path Import(const Path& path);

private:
	static Path ImportPMX(const Path& path);
};

}

