#pragma once

namespace mm
{

struct InverseKinematicsInfo;
class Bone;
class InverseKinematicsSolver
{
public:
	virtual ~InverseKinematicsSolver() = default;
	virtual void Solve(const InverseKinematicsInfo& info, Bone* target) = 0;

private:
};

}

