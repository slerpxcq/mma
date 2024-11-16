#pragma once

#include "Collider.hpp"
#include "SceneObject.hpp"

namespace mm
{

class Rigidbody : public SceneObject
{
public:
	struct ConstructInfo {
		StringView name{};
		Collider collider{};
		Transform bindWorld{};
		f32 mass{};
		f32 linearDamping{};
		f32 angularDamping{};
		f32 friction{};
		f32 restitution{};
		u16 group;
		u16 noCollisionGroupMask;
		bool isDynamic{};
	};

public:
	virtual ~Rigidbody() = default;
	Rigidbody(StringView name) :
		SceneObject{ name } {}

	void* GetHandle() const { return m_handle; }
	void SetHandle(void* handle) { m_handle = handle; }

protected:
	void* m_handle;
};

}

