#ifndef SERVICE_H_
#define SERVICE_H_

#include <functional>

class IService
{
public:
    IService(){ m_bStarted = false; }
    virtual ~IService(){}

    virtual bool Start() = 0;
    virtual bool IsStart() = 0;
    virtual bool Stop() = 0;
    virtual bool SendData(const char* data, unsigned int size) = 0;

public:
    // set callback when service accept data, <source address ip:port, data, data length> 
    void SetRecvCallback(std::function<void(const char*, const char*, int)> cb) { m_recvCb = cb; }

protected:
    std::function<void(const char*, const char*, int)> m_recvCb;
    bool                                               m_bStarted;
};

#endif
