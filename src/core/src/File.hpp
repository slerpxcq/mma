#pragma once

#include <filesystem>

namespace mm
{
	class File {
	public:
		virtual	~File() {}

	protected:
		std::filesystem::path m_path;
	};
}

