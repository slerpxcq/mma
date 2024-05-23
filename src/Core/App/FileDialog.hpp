#pragma once

#include <nfd.h>

namespace mm
{

class FileDialog {
public:
	FileDialog() = delete;
	static std::filesystem::path Open(const char* filter);
	static std::filesystem::path Save(const char* filter);
};

}

