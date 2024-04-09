#pragma once

namespace mm
{
	template <typename K, typename T>
	class Library
	{
	public:
		using Container = std::unordered_map<K, std::unique_ptr<T>>;

	public:
		void Insert(const K& key, std::unique_ptr<T> ptr) { 
			m_container.insert({ key, std::move(ptr) }); 
		}

		void Erase(const K& key) { 
			if (!m_container.erase(key)) {
				MM_ASSERT(0 && "Failed to unload resource");
			}
		}

		T* Get(const K& key) {
			T* ret = nullptr;
			auto it = m_container.find(key);
			if (it != m_container.end()) {
				ret = it->second.get();
			}
			else {
				MM_WARN("{0}: cannot find resource", __FUNCTION__);
				return nullptr;
			}
			return ret;
		}

		T* operator[](const K& key) {
			return Get(key);
		}

	private:
		Container m_container;
	};
}

