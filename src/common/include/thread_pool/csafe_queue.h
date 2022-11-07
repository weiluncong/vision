#ifndef KSAFE_QUEUE_H
#define KSAFE_QUEUE_H

#include <iostream>
#include <mutex>
#include <memory>
#include <queue>
/**
 * @brief
 * 创建安全队列用于存储任务队列及管理队列的任务变更
 */
template <typename T>
class CSafeQueue
{
public:
    CSafeQueue() {}
    CSafeQueue(CSafeQueue &&other) {}
    ~CSafeQueue() {}

    bool Empty()
    {
        std::lock_guard<std::mutex> m_lock(mutex_);
        return queue_.empty();
    }
    int Size()
    {
        std::lock_guard<std::mutex> m_lock(mutex_);
        return queue_.size();
    }
    void Enqueue(const T &t)
    {
        std::lock_guard<std::mutex> m_lock(mutex_);
        queue_.emplace(t);
    }
    bool Dequeue(T &t)
    {
        std::lock_guard<std::mutex> m_lock(mutex_);
        if (queue_.empty())
        {
            return false;
        }
        t = std::move(queue_.front());
        queue_.pop();
        return true;
    }
    void Clear()
    {
        std::lock_guard<std::mutex> m_lock(mutex_);
        if (!queue_.empty())
        {
            std::queue<T> temp;
            swap(temp, queue_);
        }
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
};

#endif // CSAFE_QUEUE_H
