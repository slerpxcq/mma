#pragma once

#include "../NamedObject.hpp"

namespace mm
{

class File : public NamedObject
{
public:
	virtual	~File() = default;
	File(const Path& path) : 
		NamedObject{ path.filename().string() },
		m_path{path} {}

	const Path& GetPath() const { return m_path; }

protected:
	const Path m_path{};
};

}

