#pragma once

namespace mm
{
	class File
	{
	public:
		//virtual const std::string& GetName() const { return std::string(); }
		//virtual const std::filesystem::path& GetPath() const { return std::filesystem::path(); }
		virtual const std::string& GetName() const = 0;
		virtual const std::filesystem::path& GetPath() const = 0;
		virtual ~File() {}
	};
}

