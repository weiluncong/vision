#include "UdpService.h"
#include <string.h>
#include "log.h"
#include "common.h"
#include <cassert>

static const int socket_recv_buf_size = 4 * 1024 * 1024;
static const int socket_send_buf_size = 4 * 1024 * 1024;

//UDP接收时指定的缓冲大小，如果缓冲过小，在接收到一次大量发送时会出现
//WSAEMSGSIZE 的错误，因此这里指定最大缓冲区大小为理论值的最大值64K(65535)
static const int UDP_RECV_BUFF_SIZE = 64 * 1024;

UdpService::UdpService(unsigned short localPort, const char* rmtIp, unsigned short rmtPort, int multicastloopback)
    : m_localPort(localPort), m_rmtIp(rmtIp), m_rmtPort(rmtPort), m_multicastloopback(multicastloopback)
{
    memset(&m_socket, 0, sizeof(m_socket));
    m_socket.handle = (~0);
    m_recvbuf = new char[UDP_RECV_BUFF_SIZE];
    assert(m_recvbuf != 0);
    m_bmulticast = IsIpMulticast(rmtIp);
}

UdpService::~UdpService()
{
    Stop();
    delete[] m_recvbuf;
    m_recvbuf = nullptr;
}

bool UdpService::Start()
{
    if (m_rmtIp.empty())
    {
        LOG_ERR("UDP connect %s:%d failed, empty dest ip!\n", m_rmtIp.c_str(), m_rmtPort);
        return false;
    }

    if (cc_socket_is_connect(&m_socket))
    {
        Stop();
    }

    if (cc_socket_init(&m_socket, false) != CC_SOCKET_OK)
    {
        LOG_ERR("UDP Init socket failed!\n");
        return false;
    }

    if (cc_socket_bind(&m_socket, m_localPort) != CC_SOCKET_OK)
    {
        LOG_ERR("UDP socket bind local port : %d failed!\n", m_localPort);
        return false;
    }

    cc_socket_set_recv_snd_buf(&m_socket, socket_recv_buf_size, socket_send_buf_size);

    //add to multicast addr if rmt is multicast ip
    AddToMulticast(m_rmtIp.c_str());

    m_stop = false;
    // recv thread
    m_recvThd = std::thread(std::bind(&UdpService::RecvThdLoop, this));
#ifdef WIN32
    thread_set_priority(m_recvThd, THREAD_PRIORITY_TIME_CRITICAL);
#endif
    m_bStarted = true;

    return true;
}

bool UdpService::IsStart()
{
    return m_bStarted;
}

bool UdpService::Stop()
{
    if (m_stop)
    {
        return true;
    }

    //remove from multicast addr if rmt is multicast ip
    RemoveFromMulticast(m_rmtIp.c_str());

    m_stop = true;
    cc_sleep(10);

    if (cc_socket_is_connect(&m_socket))
    {
        cc_socket_close(&m_socket);
    }

    if (m_recvThd.joinable())
    {
        m_recvThd.join();
    }

    m_bStarted = false;

    return true;
}


void UdpService::RecvThdLoop()
{
    while (!m_stop)
    {
        cc_socket clt;
        memset(m_recvbuf, 0, UDP_RECV_BUFF_SIZE);
        int len = cc_socket_recv_from(&m_socket, m_recvbuf, UDP_RECV_BUFF_SIZE, &clt);
        if (len > 0)
        {
            LOG_INFO("UDP recv %d bytes from %s:%d\n", len, clt.ip, clt.port);
            if (!m_bmulticast)
            {
                //UDP单播不接受非目的ip和端口的数据
                if (m_rmtIp.compare(clt.ip) != 0 ||
                    m_rmtPort != clt.port)
                {
                    LOG_INFO("UDP unicast drop %d bytes from %s:%d\n", len, clt.ip, clt.port);
                    continue;
                }
            }
            if (m_recvCb)
            {
                char src_addr[64];
                memset(src_addr, 0, sizeof(src_addr));
                snprintf(src_addr, sizeof(src_addr) - 1, "%s:%d", clt.ip, clt.port);
                m_recvCb(src_addr, m_recvbuf, len);
            }
        }
        else
        {
            cc_sleep(10);
        }
    }

    LOG_INFO("UDP recv loop exit. \n");
}

bool UdpService::IsIpMulticast(const char* rmtIp)
{
    //224.0.0.0 - 239.255.255.255
    unsigned long longaddr = cc_ntohl(cc_inet_addr(rmtIp));
    if (longaddr >= 0xe0000000 &&
        longaddr <= 0xefffffff)
    {
        return true;
    }
    return false;
}

void UdpService::AddToMulticast(const char* rmtIp)
{
    if (m_bmulticast)
    {
        cc_socket_add_membership(&m_socket, rmtIp, m_multicastloopback);
    }
}

void UdpService::RemoveFromMulticast(const char* rmtIp)
{
    if (m_bmulticast)
    {
        cc_socket_drop_membership(&m_socket, rmtIp);
    }
}

bool UdpService::SendData(const char* data, unsigned int size)
{
    LOG_INFO("UDP send %d bytes to %s:%d\n", size, m_rmtIp.c_str(), m_rmtPort);

    if (!cc_socket_is_connect(&m_socket)) {
        LOG_ERR("UDP send failed, not connect!\n");
        return false;
    }

    //ERROR_ACCESS_DENIED = 5;
    cc_socket rmt;
    strncpy(rmt.ip, m_rmtIp.c_str(), sizeof(rmt.ip));
    rmt.port = m_rmtPort;
    std::lock_guard<std::recursive_mutex> lk(m_txMutex);
    if (cc_socket_send_to(&m_socket, data, size, &rmt) != size) {
        LOG_ERR("UDP send failed!\n");
        return false;
    }

    return true;
}

