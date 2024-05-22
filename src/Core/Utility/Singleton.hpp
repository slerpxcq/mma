#pragma once

namespace mm
{
template <typename T>
class Singleton 
{
public:
	static T& Get() { return *s_instance; }

	static void Init() { 
		MM_ASSERT(!s_instance && "Instance already initialized!");
		s_instance = new T();
	}

	static void DeInit() {
		MM_ASSERT(s_instance && "Instance already de-initialized!");
		delete s_instance;
	}

protected:
	static inline T* s_instance;
};

}

