/**
 * Copyright 2024 @rubikchen
 *
 */

#ifndef CPP11_THREADPOOL_INCLUDE_THREADPOOL_H_
#define CPP11_THREADPOOL_INCLUDE_THREADPOOL_H_

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <queue>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace cpp11 {
class threadpool {
  using pool_task = std::function<void()>;

 public:
  explicit threadpool(std::size_t n) : stop_(false) {
    /**
     * n should check size, limit range
     */
    for (std::size_t i = 0; i < n; i++) {
      workers_.emplace_back(std::async(std::launch::async, [this] {
        // always run
        while (1) {
          pool_task task;
          {
            std::unique_lock<std::mutex> lk(lock_);

            // wait for stop or get a task from queue to process.
            wait_for_new_.wait(lk, [&] {
              return stop_.load(std::memory_order_acquire) || !tasks_.empty();
            });

            // ask for stop, and tasks have been processed;
            if (stop_ && tasks_.empty()) {
              return;
            }

            // get first task, and remove it.
            task = std::move(tasks_.front());
            tasks_.pop();
          }
          // execute a task;
          task();
        }
      }));
    }
  }

  virtual ~threadpool() {
    stop_ = true;

    // notify all that are waiting for job, let them finish execution
    wait_for_new_.notify_all();

    // clean wait ptr;
    for(const auto& work : workers_) {
      work.wait();
    }
  }

  template <class Func, class... Args>
  auto enqueue(Func&& f, Args&&... args)
      // std::future<result_type>, result_type is return type;
      -> std::future<typename std::result_of<Func(Args...)>::type> {
    // result type
    using return_type = typename std::result_of<Func(Args...)>::type;

    /**
     * use bind is for function like  &class::func, class_ptr, args
     *
     * */
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<Func>(f), std::forward<Args>(args)...));

    // result handle
    std::future<return_type> res_handle = task->get_future();

    {
      // add lock;
      std::unique_lock<std::mutex> lock(lock_);

      // don't allow enqueueing after stopping the pool
      if (stop_) throw std::runtime_error("enqueue on stopped ThreadPool");

      // wrap task to same type `pool_task`
      // task is a shared_ptr, lambda capture won't copy, task will add a ref.
      tasks_.emplace([task]() { (*task)(); });
    }

    // notify that there is a new task for some thread is waiting for a new
    // task; while loop;
    wait_for_new_.notify_one();

    // one time communicate handle;
    return res_handle;
  }

 private:
  // thread pool
  std::vector<std::future<void>> workers_;

  // task queue
  std::queue<pool_task> tasks_;

  // task lock, sychronize with add delete
  std::mutex lock_;

  // flag
  std::atomic<bool> stop_;

  // wait for new task
  std::condition_variable wait_for_new_;
};
}  // namespace cpp11

#endif  // CPP11_THREADPOOL_INCLUDE_THREADPOOL_H_
