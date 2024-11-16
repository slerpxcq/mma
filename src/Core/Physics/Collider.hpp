#pragma once

#include "PhysicsObject.hpp"

namespace mm
{

class Collider : public PhysicsObject
{
public:
	struct ConstructInfo {
		enum { BOX, SPHERE, CAPSULE } type;
		union {
			struct { Vec3 size; } box;
			struct { f32 radius; } sphere;
			struct { f32 radius, height; } capsule;
		} data;
	};
};

}

