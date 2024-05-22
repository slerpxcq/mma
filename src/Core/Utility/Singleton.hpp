#pragma once

namespace mm
{
template <typename T>
class Singleton 
{
public:
	static T& Get() { return *s_instance; }
	virtual void Startup() {}
	virtual void Shutdown() {}

	static void Init() { 
		MM_ASSERT(!s_instance && "Instance already initialized!");
		s_instance = new T();
		s_instance->Startup();
	}

	static void DeInit() {
		MM_ASSERT(s_instance && "Instance already de-initialized!");
		s_instance->Shutdown();
		delete s_instance;
	}

protected:
	static inline T* s_instance;
};

}

