#pragma once

#include "Base.hpp"

namespace mm
{

template <typename T>
class Singleton 
{
public:
	Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&) = delete;

	static T& Instance() { return *s_instance; }
	virtual void Startup() {}
	virtual void Shutdown() {}

	static void CreateInstance() { 
		MM_ASSERT(!s_instance && "Instance already created!");
		s_instance = new T();
		s_instance->Startup();
	}

	static void DestroyInstance() {
		MM_ASSERT(s_instance && "Instance already destroyed");
		s_instance->Shutdown();
		delete s_instance;
	}

protected:
	static inline T* s_instance{};
};

}
