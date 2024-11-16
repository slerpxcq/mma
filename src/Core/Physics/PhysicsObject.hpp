#pragma once

namespace mm
{

class PhysicsObject
{
public:
	PhysicsObject() = default;
	virtual ~PhysicsObject() = default;
	void* GetHandle() const { return m_handle; }
	void SetHandle(void* handle) { m_handle = handle; }

protected:
	void* m_handle{};
};

}

