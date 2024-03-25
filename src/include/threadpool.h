#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool
{
public:
  ThreadPool(int n) : nThreads(n), stop(false)
  {
    nThreads = n;
    std::cout << "Creating thread pool with " << nThreads << " threads" << std::endl;
    for (int i = 0; i < nThreads; ++i)
    {
      threads.emplace_back([this]
                           {
                for (;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                } });
    }
  }

  void enqueue(std::function<void()> task)
  {
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      tasks.emplace(std::move(task));
    }
    condition.notify_one();
  }

  template <typename Func, typename... Args>
  void enqueue(Func &&func, Args &&...args)
  {
    auto task = std::make_shared<std::packaged_task<void()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      tasks.emplace([task]()
                    { (*task)(); });
    }
    condition.notify_one();
  }

  void clear()
  {
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : threads)
      worker.join();
  }

  void resetThreads()
  {
    threads.clear();
    for (int i = 0; i < nThreads; ++i)
    {
      threads.emplace_back([this]
                           {
                for (;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                } });
    }
    // Reset the stop flag
    stop = false;
    // Reset the queue
    std::queue<std::function<void()>> empty;
    std::swap(tasks, empty);
    std::cout << "Resetting thread pool with " << nThreads << " threads" << std::endl;
  }

  ~ThreadPool()
  {
    clear();
  }

  int getNumberOfThreads() const
  {
    return nThreads;
  }

  void setNumberOfThreads(int n)
  {
    nThreads = n;
  }

private:
  std::vector<std::thread> threads;
  std::queue<std::function<void()>> tasks;

  std::mutex queueMutex;
  std::condition_variable condition;
  int nThreads;
  bool stop;
};