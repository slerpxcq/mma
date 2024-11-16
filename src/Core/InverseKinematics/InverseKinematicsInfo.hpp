#pragma once

namespace mm
{

class Bone;
struct InverseKinematicsInfo
{
	struct Node {
		Bone* bone{};
		Opt<Pair<Vec3, Vec3>> limit{};
	};

	i32 iteration{};
	f32 unitAngle{};
	DynArray<Node> chain;
};

}

