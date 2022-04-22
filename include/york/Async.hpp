/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2022 Matthew McCall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
