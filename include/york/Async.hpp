#if !defined(YORK_ASYNC_HPP)
#define YORK_ASYNC_HPP

#include <functional>

#include <taskflow/taskflow.hpp>

namespace york::async {

tf::Executor& getExecutor();

template <typename T>
using Future = tf::Future<T>;

template <typename T, typename... ArgsT>
auto dispatch(T func, ArgsT&&... args)
{
    return getExecutor().async(func, std::forward<ArgsT>(args)...);
}

template <typename... ArgsT>
void dispatch(std::function<void()> func, ArgsT&&... args)
{
    return getExecutor().silent_async(func, std::forward<ArgsT>(args)...);
}

} // namespace york::async

#endif // YORK_ASYNC_HPP
