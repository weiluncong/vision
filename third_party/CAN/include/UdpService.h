#ifndef UDP_SERVICE_H_
#define UDP_SERVICE_H_


#include "Service.h"
#include "network.h"
#include <thread>
#include <vector>
#include <mutex>

class UdpService : public IService
{
public:
    UdpService(unsigned short localPort, const char* rmtIp, unsigned short rmtPort, int multicastloopback);
    virtual ~UdpService();

    virtual bool Start();
    virtual bool IsStart();
    virtual bool Stop();
    virtual bool SendData(const char* data, unsigned int size);

public:
    void RecvThdLoop();

private:
    bool IsIpMulticast(const char* rmtIp);
    void AddToMulticast(const char* rmtIp);
    void RemoveFromMulticast(const char* rmtIp);

private:
    unsigned short m_localPort = 0;
    std::string    m_rmtIp;
    unsigned short m_rmtPort;
    int            m_multicastloopback{ 1 };
    bool           m_bmulticast{ false };

    cc_socket m_socket;
    std::thread m_recvThd;
    bool m_stop = false;
    std::recursive_mutex m_txMutex;

    char*       m_recvbuf = nullptr;
};


#endif //UDP_SERVICE_H_


