#if !defined(YORK_ASYNC_HPP)
#define YORK_ASYNC_HPP

#include <functional>

#include <taskflow/taskflow.hpp>

namespace york::async {

tf::Executor& getExecutor();

template <typename T>
using Future = tf::Future<T>;

template <typename T>
auto dispatch(std::function<T()> func)
{
    return getExecutor().async(func);
}

} // namespace york::async

#endif // YORK_ASYNC_HPP
