#pragma once

#include "File.hpp"

namespace mm
{

class Text : public File
{
public:
	explicit Text(const Path& path);
	const String& GetString() const { return m_string; }


private:
	String m_string{};
};

}

