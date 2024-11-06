#pragma once

#include "Common/Math/Transform.hpp"
#include "NamedObject.hpp"
#include "SceneObject.hpp"

/* Note: 
 * To ensure world transform is calculated correctly, world transform of parent
 * node should be calculated correctly.
 */

namespace mm
{

class SceneObject;

class Node : public NamedObject
{
public:
	// virtual ~Node() = default;
	Node(StringView name) : NamedObject{ name } {}

	void SetLocalTransform(const Transform& transform);
	void SetWorldTransform(const Transform& transform);
	void SetWorldTranslation(const Vec3& translation);
	void SetWorldRotation(const Quat& rotation);
	void SetLocalTranslation(const Vec3& translation);
	void SetLocalRotation(const Quat& rotation);
	void TransformWorld(const Transform& transform);
	void TransformLocal(const Transform& transform);
	void TranslateLocal(const Vec3& translation);
	void TranslateWorld(const Vec3& translation);
	void RotateLocal(const Quat& rotation);
	void RotateWorld(const Quat& rotation);

	Transform GetLocalTransform() const { return m_localTransform; }
	Transform GetWorldTransform() const { return m_worldTransform; }
	Mat4 GetLocalMatrix() const { return m_localTransform.ToMat4(); }
	Mat4 GetWorldMatrix() const { return m_worldTransform.ToMat4(); }

	auto& GetChildren() { return m_children; }
	Node* SearchChild(StringView name);

	Node& AddChild(StringView name) {
		m_children.emplace_back(new Node{ name });
		m_children.back()->m_parent = this;
		return *m_children.back();
	}

	// void AttachObject(Ref<SceneObject> obj) {
	// 	m_object = obj;
	// 	obj->AttachTo(*this);
	// }

	void UpdateSubtreeWorldTransform();

private:
	void CalculateLocalTransform();

private:
	const String m_name{};
	Node* m_parent{};
	DynArray<Scoped<Node>> m_children{};

	Transform m_localTransform{};
	Transform m_worldTransform{};

	Ref<SceneObject> m_object{};
};

}

