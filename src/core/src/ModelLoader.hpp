#pragma once

#include "ModelNode.hpp"
#include "PMXFile.hpp"

namespace mm
{

class ModelLoader
{
public:
	ModelLoader() = delete;
	static std::shared_ptr<ModelNode> LoadFromPMX(std::shared_ptr<PMXFile> pmx);

private:
};

}

