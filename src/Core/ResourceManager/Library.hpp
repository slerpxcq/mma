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
			try {
				ret = m_container.at(key).get();
			}
			catch (const std::out_of_range& e) {
				//MM_WARN("Get(): element \"{0}\" does not exist", key);
				MM_ASSERT(0 && "Failed to get resource");
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

