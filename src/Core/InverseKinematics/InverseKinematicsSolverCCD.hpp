#pragma once

#include "InverseKinematicsSolver.hpp"

namespace mm
{

struct InverseKinematicsInfo;
class InverseKinematicsSolverCCD : public InverseKinematicsSolver
{
public:
	virtual void Solve(const InverseKinematicsInfo& info, const Bone* target) override;
};

}

