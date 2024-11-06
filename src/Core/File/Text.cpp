#include "CorePch.hpp"
#include "Text.hpp"

namespace mm
{

Ref<Text> Text::Load(const Path& path)
{
	auto text = Ref<Text>(new Text{ path });
	InFileStream ifs;
	ifs.exceptions(ifs.badbit | ifs.failbit);
	ifs.open(path);
	text->m_string = String{ std::istreambuf_iterator<char>(ifs),
							 std::istreambuf_iterator<char>() };
	return text;
}

}
