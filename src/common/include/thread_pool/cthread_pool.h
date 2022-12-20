#ifndef CTHREAD_POOL_H
#define CTHREAD_POOL_H

#include <iostream>
#include <functional>
#include <future>
#include <thread>
#include "csafe_queue.h"

/**
 * @brief
 * 创建线程池
 */
class CThreadPool
{

private:
    /**
     * @brief
     * 定义线程worker的运行函数，传入参数为线程池以及该线程的id
     */
    class CThreadWorker
    {
    private:
        int id_;
        CThreadPool *pool_;

    public:
        CThreadWorker(CThreadPool *pool, int id)
            : pool_(pool), id_(id) {}
        ~CThreadWorker() {}
        void operator()()
        {
            std::function<void()> func;
            while (!pool_->shut_down_)
            {
                {
                    std::unique_lock<std::mutex> m_lock(pool_->conditional_mutex_);
                    if (pool_->queue_.Empty())
                    {
                        pool_->conditional_lock_.wait(m_lock);
                    }
                }
                if (pool_->queue_.Dequeue(func))
                {
                    func();
                }
            }
        }
    };

private:
    bool shut_down_;

    std::vector<std::thread> threads_;
    std::mutex conditional_mutex_;             // 线程互斥锁
    std::condition_variable conditional_lock_; // 线程环境锁，判断当前锁是否lock及是否需要唤醒

public:
    CThreadPool(int n_threads = 1)
        : threads_(std::vector<std::thread>(n_threads)), shut_down_(false)
    {
        Init();
    }

    ~CThreadPool() {}

    CThreadPool(const CThreadPool &) = delete;

    CThreadPool &operator=(const CThreadPool &) = delete;

public:
    void Init();
    void ShutDown();
    CSafeQueue<std::function<void()>> queue_;

    /**
     * @brief
     * typename... Args为C++11的不定长数量变量声明
     * 第二句为尾置声明方式，其中&&为万能引用，std::future为多线程异步时对异步操作结果进行监控访问的机制
     * decltype为C++11的自动类型推导关键字
     * std::forward为完美转发，如果类型参数为左值，则转发为左值，反之亦然
     * std::packged_task为打包一个对象可以供std::future调用，获取线程异步操作
     */
    template <typename T, typename... Args>
    auto submit(T &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        std::function<decltype(f(args...))()> func = std::bind(std::forward<T>(f), std::forward<Args>(args)...);
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
        // 包装工作指针到std::function
        std::function<void()> warpper_func = [task_ptr]()
        {
            (*task_ptr)();
        };
        queue_.Enqueue(warpper_func);
        conditional_lock_.notify_one();
        return task_ptr->get_future();
    }
};

#endif // CTHREAD_POOL_H
