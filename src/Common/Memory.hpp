#pragma once

#include <memory>

namespace mm
{

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
decltype(auto) MakeRef(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename D = std::default_delete<T>>
using Scoped = std::unique_ptr<T, D>;

template <typename T, typename... Args>
decltype(auto) MakeScoped(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

};




