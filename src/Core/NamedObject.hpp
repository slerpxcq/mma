#pragma once

namespace mm
{

class NamedObject
{
public:
	virtual ~NamedObject() = default;
	NamedObject(StringView name) : 
		m_name{ name } {}

	StringView GetName() const { return m_name; }

protected:
	const String m_name{};
};

}

