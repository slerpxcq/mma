#include "CorePch.hpp"
#include "Rigidbody.hpp"

#include "Node.hpp"
#include "Bone.hpp"

#include "PhysicsManager.hpp"
#include "SceneManager.hpp"

namespace mm
{

void Rigidbody::PullBoneTransform(Transform::Type type)
{
	auto rigidbodyToBone = m_bone->GetBindWorldInverse() * m_bindWorld;
	auto transform = m_bone->GetNode()->GetWorldTransform() * rigidbodyToBone;
	GetPhysicsManager()->PushRigidbodyTransform(this, transform, type);
}

void Rigidbody::PushBoneTransform(Transform::Type type)
{
	GetPhysicsManager()->PullRigidbodyTransform(this);
	auto boneToRigidbody = m_bindWorld.Inverse() * m_bone->GetBindWorld();
	auto transform = m_bone->GetNode()->GetWorldTransform() * boneToRigidbody;
	if (type & Transform::Type::ROTATION_BIT) {
		m_bone->GetNode()->SetWorldRotation(transform.rotation);
	}
	if (type & Transform::Type::TRANSLATION_BIT) {
		m_bone->GetNode()->SetWorldTranslation(transform.translation);
	}
}

Rigidbody::Builder::Builder(const Rigidbody::ConstructInfo& info)
{
	m_rigidbody = GetSceneManager()->CreateObject<Rigidbody>(info.name);
}

}
