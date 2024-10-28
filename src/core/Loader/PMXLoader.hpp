#pragma once

#include "File/PMXFile.hpp"
#include "../ModelNode.hpp"

namespace mm
{

/* GOAL: Generate a ModelNode from PMXFile */

class PMXLoader
{
public:
	Scoped<ModelNode> Load(const PMXFile& pmx);

private:
	void LoadMesh(const PMXFile& pmx);

private:
	Scoped<ModelNode> m_model{};
};

}


