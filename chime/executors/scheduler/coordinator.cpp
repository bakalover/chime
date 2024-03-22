#include <chime/executors/scheduler/coordinator.hpp>
#include <cstdint>

namespace executors {

uint64_t Coordinator::NumSearch() { return state_.load() & search_mask_; }

bool Coordinator::TrySpin() {
  auto state = state_.load();
  if ((state & search_mask_) * 2 >= threads_) {
    return false;
  }
  state_.fetch_add(1);
  return true;
}

bool Coordinator::StopSpin() {
  auto state = state_.fetch_sub(1);
  return (state & search_mask_) == 1;
}

} // namespace executors
