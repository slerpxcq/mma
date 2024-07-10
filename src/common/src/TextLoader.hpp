#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Base.hpp"

class TextLoader
{
public:
	TextLoader() = delete;

	static std::string Load(const std::filesystem::path& path) {
		std::ifstream ifs(path);
		if (!ifs.is_open()) {
			MM_ERROR("Failed to load text file; path={0}", path.string());
			MM_ASSERT(0);
		}
		std::stringstream ss;
		ss << ifs.rdbuf();
		std::string str = ss.str();
		return str;
	}
};

