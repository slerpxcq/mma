#pragma once

#include "NamedObject.hpp"

namespace mm
{

class Resource : public NamedObject
{
public:
	Resource(StringView name) : 
		NamedObject{ name } {}

};

}

