//
// Created by Matthew McCall on 5/3/22.
//

#ifndef YORK_CONTAINERS_HPP
#define YORK_CONTAINERS_HPP

#include <vector>

#include <boost/pool/pool_alloc.hpp>
#include <boost/circular_buffer.hpp>

namespace york {

template <typename T>
using Allocator = boost::pool_allocator<T>;

template <typename T>
using Vector = std::vector<T, Allocator<T>>;

template <typename T>
using CircularBuffer = boost::circular_buffer<T, Allocator<T>>;

}

#endif // YORK_CONTAINERS_HPP
