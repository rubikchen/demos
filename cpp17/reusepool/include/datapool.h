/**
 * Copyright 2024 @rubikchen
 *
 */

#ifndef CPP17_REUSEPOOL_INCLUDE_DATAPOOL_H_
#define CPP17_REUSEPOOL_INCLUDE_DATAPOOL_H_

#include <atomic>
#include <memory>
#include <mutex>
#include <queue>

namespace cpp17 {

template <typename T>
class data_pool {
  using _TSPtr = std::shared_ptr<T>;
  using _TUPtr = std::unique_ptr<T>;

 public:
  data_pool() = default;
  ~data_pool() { clear(); }
  bool empty() { return pool_.empty(); }

  template <typename... _Args>
  bool make_item(_Args&&... args) {
    return enqueue(std::make_unique<T>(std::forward<_Args>(args)...));
  }

  bool enqueue(_TUPtr t) {
    std::lock_guard<std::mutex> lk(lock_);
    if (t) {
      pool_.push(
          _TSPtr(t.release(), [this, magic = this->magic_num_.load()](T* t) {
            if (magic != this->magic_num_) {
              delete t;
            } else {
              this->enqueue(_TUPtr(t));
            }
          }));
      return true;
    } else {
      return false;
    }
  }
  _TSPtr get() {
    std::lock_guard<std::mutex> lk(lock_);
    _TSPtr ret = pool_.front();
    if (ret) {
      pool_.pop();
    }
    return ret;
  }

  void clear() {
    magic_num_++;
    while (!empty()) {
      pool_.pop();
    }
  }

 private:
  std::queue<_TSPtr> pool_;
  std::mutex lock_;
  std::atomic<int> magic_num_{0};
};

}  // namespace cpp17

#endif  // CPP17_REUSEPOOL_INCLUDE_DATAPOOL_H_
