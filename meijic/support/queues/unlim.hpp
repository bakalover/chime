#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
namespace queue::supp {
template <typename T> class MPMCUnlimitedQueue {
public:
  bool Put(T val);

  std::optional<T> Take();

  // Closing queue for submitting. Remainging tasks available for taking
  void Close();

private:
  // TODO: intrusive list
  std::queue<T> q_;
  // TODO: Lock-free
  std::mutex mutex_;
  std::condition_variable cond_;
  bool is_open_ = true;
};

} // namespace supp