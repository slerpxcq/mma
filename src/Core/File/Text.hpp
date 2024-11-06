#pragma once

#include "File.hpp"

namespace mm
{

class Text : public File
{
public:
	static Ref<Text> Load(const Path& path);
	const String& GetString() const { return m_string; }

private:
	Text(const Path& path) : File{ path } {}

private:
	String m_string{};
};

}

