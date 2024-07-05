#pragma once

namespace mm
{

class Node
{
public:
	Node(std::shared_ptr<Node> parent = nullptr) : m_parent(parent) {}
	void AddChild(std::shared_ptr<Node> child) {
		m_children.push_back(std::move(child));
	}
	auto& GetChildren() { return m_children; }

protected:
	std::weak_ptr<Node> m_parent;
	std::vector<std::shared_ptr<Node>> m_children;

	std::string m_name;
	uint64_t m_id;
	Transform m_localTransform;
};

}

