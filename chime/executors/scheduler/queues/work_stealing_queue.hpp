#pragma once

#include "chime/support/queues/steal.hpp"
#include <chime/executors/task.hpp>
#include <cstddef>
#include <span>
#include <wheels/intrusive/forward_list.hpp>

namespace executors {

template <typename T, size_t Cap>
using WorkStealingQueue = support::queue::StealingQueue<T, Cap>;

}