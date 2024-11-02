#pragma once

#include "Common/Math/Transform.hpp"

/* Note: 
 * To ensure world transform is calculated correctly, world transform of parent
 * node should be calculated correctly.
 */

namespace mm
{

class Node
{
	// TODO: remove friendship
	friend class Application;
public:
	const String& GetName() { return m_name; }
	void SetLocalTransform(const Transform& transform) { m_localTransform = transform; }
	Transform GetLocalTransform() const { return m_localTransform; }
	Transform GetWorldTransform() const { return m_worldTransform; }
	void UpdateSubtreeWorldTransform();
	Node* SearchChild(StringView name);
	auto& GetChildren() { return m_children; }

	template <typename T, typename... Args>
	Node& AddChild(Args&&... args) {
		m_children.emplace_back(Scoped<T>{new T{ std::forward<Args>(args)... }});
		auto& child = m_children.back();
		child->m_parent = this;
		return *child;
	}

	Node& AttachChild(Scoped<Node> child) {
		child->m_parent = this;
		m_children.push_back(std::move(child));
		return *m_children.back();
	}

	virtual ~Node() = default;

protected:
	Node(const String& name) : m_name{ name } {}

private:
	const String m_name{};
	const Node* m_parent{};
	DynArray<Scoped<Node>> m_children{};

	Transform m_localTransform{};
	Transform m_worldTransform{};
};

}

