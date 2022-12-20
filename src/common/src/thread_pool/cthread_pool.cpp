#include "cthread_pool.h"

// CThreadPool *CThreadPool::thread_pool_ = GetCThreadPool();

/**
 * @brief
 * 初始化线程池，添加线程worker
 */
void CThreadPool::Init()
{
    // std::cout << threads_.size() << std::endl;
    for (unsigned int i = 0; i < threads_.size(); ++i)
    {
        threads_[i] = std::thread(CThreadWorker(this, i));
    }
}

void CThreadPool::ShutDown()
{
    shut_down_ = true;
    conditional_lock_.notify_all();
    for (auto &i : threads_)
    {
        if (i.joinable())
        {
            i.join();
        }
    }
}
