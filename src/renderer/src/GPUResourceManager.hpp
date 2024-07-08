#pragma once

#include "GL/GPUResource.hpp"

namespace mm
{

class GPUResourceManager : public Singleton<GPUResourceManager>
{
public:
	virtual void Startup() override;
	virtual void Shutdown() override;

	template <typename T>
	void Load(const std::string& name, std::shared_ptr<T> ptr) {
		m_resources.insert({ name, std::weak_ptr<T>(ptr) });
	}

	template <typename T, typename... Ts>
	std::shared_ptr<T> Load(const std::string& name, Ts&&... ts) {
		auto it = m_resources.find(name);
		bool exist = it != m_resources.end();
		std::shared_ptr<T> ret;
		if (exist) {
			if (it->second.expired()) {
				ret = std::make_shared<T>(std::forward<Ts>(ts)...);
				it->second = ret;
			} else {
				ret = std::dynamic_pointer_cast<T>(it->second.lock());
			}
		} else {
			ret = std::make_shared<T>(std::forward<Ts>(ts)...);
			m_resources.insert({ name, std::weak_ptr<T>(ret) });
		}
		return ret;
	}

	template <typename T>
	std::shared_ptr<T> Get(const std::string& name) {
		auto it = m_resources.find(name);
		if (it != m_resources.end()) {
			return it->second.lock();
		} else {
			return nullptr;
		}
	}

private:
	std::unordered_map<std::string, std::weak_ptr<GPUResource>> m_resources;
};

}
