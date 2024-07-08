#pragma once

#include "Node.hpp"

namespace mm
{

struct Collider 
{
	virtual	~Collider() {}
};

struct SphereCollider : public Collider
{
	float radius;
};

struct CapsuleCollider : public Collider
{
};

struct BoxCollider : public Collider 
{
};

class RigidbodyNode : public Node
{
public:
	RigidbodyNode(const std::string& name = "", 
			   std::shared_ptr<Node> parent = nullptr) :
		Node(name, parent) {}

private:
	std::shared_ptr<Collider> m_collider;
};

}

