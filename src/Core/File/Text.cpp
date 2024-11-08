#include "CorePch.hpp"
#include "Text.hpp"

namespace mm
{

Text::Text(const Path& path) :
	File{ path }
{
	InFileStream ifs;
	ifs.exceptions(ifs.badbit | ifs.failbit);
	ifs.open(path);
	m_string = String{ std::istreambuf_iterator<char>(ifs),
					   std::istreambuf_iterator<char>() };
}

}
