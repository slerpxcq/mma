#pragma once

namespace mm
{
	class File {
	public:
		virtual	~File() {}
		virtual std::string GetName() const = 0;
		virtual std::filesystem::path GetPath() const = 0;
	protected:
		std::filesystem::path m_path;
	};

}
