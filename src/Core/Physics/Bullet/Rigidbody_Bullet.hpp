#pragma once

#include "../Rigidbody.hpp"

namespace mm
{

class Rigidbody_Bullet : public Rigidbody
{
public:
	Rigidbody_Bullet(StringView name) :
		Rigidbody{ name } {}

private:
};

}

