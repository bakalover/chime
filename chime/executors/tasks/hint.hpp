#pragma once

namespace executors {

enum class SchedulerHint {
  UpToYou = 1, // Rely on executor scheduling decision
  Next = 2     // LIFO
};

} // namespace executors
