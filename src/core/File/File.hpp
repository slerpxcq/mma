#pragma once

namespace mm
{

class File
{
public:
	File(const Path& path) : 
		m_path{path}, 
		m_name{ path.filename().string() } {}
	virtual	~File() = default;

	const Path& GetPath() const { return m_path; }
	const String& GetName() const { return m_name; }

protected:
	const Path m_path{};
	const String m_name{};
};

}

