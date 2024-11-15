#include "CorePch.hpp"
#include "InverseKinematicsSolverCCD.hpp"

#include "../Bone.hpp"
#include "../Node.hpp"
#include "InverseKinematicsInfo.hpp"

namespace mm
{

void InverseKinematicsSolverCCD::Solve(const InverseKinematicsInfo& info, const Bone* target)
{
	for (u32 iteration = 0; iteration < info.iteration; ++iteration) {
		auto& link = info.link;
		for (auto ikNode = link.cbegin()+1; ikNode != link.cend(); ++ikNode) {
			Bone* bone = ikNode->bone;
			Vec3 o = bone->GetNode()->GetWorldTranslation();
			Vec3 E = link.front().bone->GetNode()->GetWorldTranslation();
			Vec3 T = target->GetNode()->GetWorldTranslation();
			Vec3 t = glm::normalize(T - o);
			Vec3 e = glm::normalize(E - o);
			Quat qWorld = glm::rotation(e, t);
			Quat qLocalToWorld = bone->GetNode()->GetWorldRotation(); 
			Quat qLocal = glm::inverse(qLocalToWorld) * qWorld * qLocalToWorld;

			if (ikNode->hasLimit) {
				Vec3 eulerIdeal = glm::eulerAngles(ikNode->bone->GetNode()->GetLocalRotation() * qLocal);
				Vec3 eulerClamped = glm::clamp(eulerIdeal, ikNode->lowerLimit, ikNode->upperLimit);
				Quat qClamped = Quat(eulerClamped);
				qLocal = glm::inverse(ikNode->bone->GetNode()->GetLocalRotation()) * qClamped;
			}

			ikNode->bone->GetNode()->RotateLocal(qLocal);
		}
	}
}

}
