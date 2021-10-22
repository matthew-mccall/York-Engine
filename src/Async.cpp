#include <york/Async.hpp>

namespace {
static tf::Executor s_executor;
} // namespace

namespace york::async {

tf::Executor& getExecutor()
{
    return s_executor;
}

} // namespace york::async
