#pragma once

#include "Common/Math/Transform.hpp"
#include "SceneObject.hpp"

/* Note: 
 * To ensure world transform is calculated correctly, world transform of parent
 * node should be calculated correctly.
 */

namespace mm
{

class SceneObject;

class Node
{
	// TODO: remove friendship
	friend class Application;
public:
	virtual ~Node() = default;
	Node(StringView name) : m_name{ name } {}
	const String& GetName() { return m_name; }

	void SetLocalTransform(const Transform& transform) { m_localTransform = transform; }
	void SetTranslation(Vec3 translation) { m_localTransform.translation = translation; }
	void SetRotation(Quat q) { m_localTransform.rotation = q; }
	void Rotate(Quat q) { m_localTransform.rotation = q * m_localTransform.rotation; }
	void Translate(Vec3 translation) { m_localTransform.translation += translation; }
	Transform GetLocalTransform() const { return m_localTransform; }
	Transform GetWorldTransform() const { return m_worldTransform; }
	Mat4 GetLocalMatrix() const { return m_localTransform.ToMat4(); }
	Mat4 GetWorldMatrix() const { return m_worldTransform.ToMat4(); }
	void UpdateSubtreeWorldTransform();

	Node* SearchChild(StringView name);
	auto& GetChildren() { return m_children; }

	Node& AddChild(StringView name) {
		m_children.emplace_back(new Node{ name });
		return *m_children.back();
	}

	void AttachObject(Ref<SceneObject> obj) {
		m_object = obj;
		obj->AttachTo(*this);
	}

private:
	const String m_name{};
	const Node* m_parent{};
	DynArray<Scoped<Node>> m_children{};

	Transform m_localTransform{};
	Transform m_worldTransform{};

	Ref<SceneObject> m_object{};
};

}

