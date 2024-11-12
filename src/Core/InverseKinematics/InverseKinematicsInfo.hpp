#pragma once

namespace mm
{

class Bone;
struct InverseKinematicsInfo
{
	struct Node {
		Bone* bone{};
		bool hasLimit{};
		Vec3 upperLimit{};
		Vec3 lowerLimit{};
	};

	i32 iteration{};
	f32 unitAngle{};
	DynArray<Node> link;
};

}

