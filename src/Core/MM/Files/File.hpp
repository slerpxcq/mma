#pragma once

namespace mm
{
	class File
	{
	public:
		virtual const std::string& GetName() const { return ""; }
		virtual const std::filesystem::path& GetPath() const { return ""; }
		virtual ~File() {}
	};
}

