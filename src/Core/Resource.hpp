#pragma once

namespace mm
{

class Resource
{
public:
	Resource(StringView name) : m_name{ name } {}
	StringView GetName() const { return m_name; }

private:
	const String m_name{};
};

}

