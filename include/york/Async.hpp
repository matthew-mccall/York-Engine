#if !defined(YORK_ASYNC_HPP)
#define YORK_ASYNC_HPP

#include <functional>

#include <taskflow/taskflow.hpp>

namespace york::async {

tf::Executor& getExecutor();

template <typename T>
using Future = tf::Future<T>;

/**
 * @brief Executes func asynchronously
 *
 * @tparam F The function tye
 * @tparam ArgsT The parameter pack
 * @param func The function to dispatch
 * @param args The arguments to be passed to func
 * @return auto A future containing the return value from func
 */
template <typename F, typename... ArgsT>
auto dispatch(F&& func, ArgsT&&... args)
{
    return getExecutor().async(func, std::forward<ArgsT>(args)...);
}

/**
 * @brief Non-Return Dispatch. Executes a function, which returns void, asynchronously.
 *
 * @tparam F The function type.
 * @tparam ArgsT The parameter pack.
 * @param func The function to dispatch.
 * @param args The arguments to be passed to func.
 */
template <typename F, typename... ArgsT>
void nrDispatch(F&& func, ArgsT&&... args)
{
    getExecutor().silent_async(func, std::forward<ArgsT>(args)...);
}

} // namespace york::async

#endif // YORK_ASYNC_HPP
