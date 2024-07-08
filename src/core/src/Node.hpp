#pragma once

namespace mm
{

class Node : public std::enable_shared_from_this<Node>
{
public:
	Node(const std::string& name = "", 
		 std::shared_ptr<Node> parent = nullptr) : 
		m_name(name),
		m_parent(parent) {}

	auto& GetChildren() { return m_children; }
	/* Overload for type checking */
	virtual void AddChild(std::shared_ptr<Node> child) {
		child->m_parent = weak_from_this();
		m_children.push_back(child);
	}

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() { return m_name; }

	void SetLocalTransform(const Transform& transform) { m_localTransform = transform; }
	Transform GetWorldTransform() const { return m_worldTransform; };
	void UpdateWorldTransform();
	virtual ~Node() {}

protected:
	std::weak_ptr<Node> m_parent;
	std::vector<std::shared_ptr<Node>> m_children;

	std::string m_name;
	uint64_t m_id;

	Transform m_localTransform;
	Transform m_worldTransform;
};

}

