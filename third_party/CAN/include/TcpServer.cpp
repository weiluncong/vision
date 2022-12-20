#include "TcpServer.h"
#include <string.h>
#include "log.h"
#include "common.h"
#include <cassert>

static const int socket_recv_buf_size = 4 * 1024 * 1024;
static const int socket_send_buf_size = 4 * 1024 * 1024;

static const int app_recv_buf_size = 64 * 1024;

TcpServer::TcpServer(unsigned short svrPort, unsigned int maxClient) 
: m_svrPort(svrPort)
, m_maxClient(maxClient)
{
    memset(&m_socket, 0, sizeof(m_socket));
    m_socket.handle = (~0);

    m_recvbuf = new char[app_recv_buf_size];
    assert(m_recvbuf != 0);
}

TcpServer::~TcpServer()
{
    Stop();

    delete[] m_recvbuf;
    m_recvbuf = nullptr;
}

bool TcpServer::Start()
{
    Stop();

    if (cc_socket_init(&m_socket) != CC_SOCKET_OK) {
        return false;
    }

    cc_socket_set_recv_snd_buf(&m_socket, socket_recv_buf_size, socket_send_buf_size);

    if (cc_socket_bind(&m_socket, m_svrPort) != CC_SOCKET_OK) {
        return false;
    }

    if(cc_socket_listen(&m_socket, m_maxClient) != CC_SOCKET_OK) {
        return false;
    }

    m_stop = false;

    // accept thread
    m_acceptThd = std::thread(std::bind(&TcpServer::AcceptThdLoop, this));

    // recv thread
    m_recvThd = std::thread(std::bind(&TcpServer::RecvThdLoop, this));

#ifdef WIN32
    thread_set_priority(m_recvThd, THREAD_PRIORITY_TIME_CRITICAL);
#endif
    m_bStarted = true;

    LOG_INFO("Tcp server %s:%d started.\n", m_socket.ip, m_socket.port);

    return true;
}

bool TcpServer::IsStart()
{
    return m_bStarted;
}

bool TcpServer::Stop()
{
    m_stop = true;
    cc_sleep(10);

    // close all client
    {
        std::unique_lock<std::recursive_mutex> lk(m_clientsMutex);
        for (auto &c : m_clients) {
            cc_socket_close(&c);
        }
        m_clients.clear();
    }

    if (cc_socket_is_connect(&m_socket)) {
        cc_socket_close(&m_socket);
    }

    if (m_acceptThd.joinable()) {
        m_acceptThd.join();
    }

    if (m_recvThd.joinable()) {
        m_recvThd.join();
    }

    m_bStarted = false;

    return true;
}

void TcpServer::AcceptThdLoop()
{
#ifdef WIN32
	LOG_ALWAYS("TcpServer AcceptThdLoop thread,tid = %u\n", GetCurrentThreadId());
#endif
    while (!m_stop) 
    {
        cc_socket clt;
        memset(&clt, 0, sizeof(clt));
        if (cc_socket_accept(&m_socket, &clt) == CC_SOCKET_OK) {
            std::unique_lock<std::recursive_mutex> lk(m_clientsMutex);
            m_clients.push_back(clt);
            LOG_INFO("Client %s:%d connected.\n", clt.ip, clt.port);
        }
        else {
            cc_sleep(10);
        }
    }
}

void TcpServer::RecvThdLoop()
{
#ifdef WIN32
	LOG_ALWAYS("TcpServer RecvThdLoop thread,tid = %u\n", GetCurrentThreadId());
#endif
    while (!m_stop)
    {
        std::vector<cc_socket> setted;
        int ret = cc_socket_select(&m_socket, &m_clients, &setted, 0); // -1 block, 0 return immediately
        if (ret == 0) {
            // the time limit expired
            cc_sleep(1);
        }
        else if (ret < 0) {
            // an error occurred
            cc_sleep(1000);
        }
        else {
            for (unsigned int i=0; i<setted.size(); i++) {
                std::unique_lock<std::recursive_mutex> lk(m_clientsMutex);
                for (unsigned int j=0; j<m_clients.size(); j++) {
                    if (m_clients[j].handle == setted[i].handle) {
                        memset(m_recvbuf, 0, app_recv_buf_size);
                        cc_socket *curSock = &m_clients[j];
                        int len = cc_socket_recv(curSock, m_recvbuf, app_recv_buf_size);
                        if (len < 0) {
                            // disconnected
                            LOG_INFO("%s:%d disconnected.\n", curSock->ip, curSock->port);
                            cc_socket_close(curSock);
                            m_clients.erase(m_clients.begin() + j); 
                        }
                        else if (len == 0) {
                        }
                        else {
                            //LOG_INFO("TCP recv %d bytes from %s:%d.\n", len, curSock->ip, curSock->port);
                            if (m_recvCb) {
                                char src_addr[64];
                                memset(src_addr, 0, sizeof(src_addr));
                                snprintf(src_addr, sizeof(src_addr) - 1, "%s:%d", curSock->ip, curSock->port);
                                m_recvCb(src_addr, m_recvbuf, len);
                            } 
                        }
                        break;
                    }
                }
            }
        }
    }
}

bool TcpServer::SendData(const char* data, unsigned int size)
{
    if (m_clients.size() == 0) {
        return false;
    }

    LOG_INFO("TCP send %d bytes to %d clients ...\n", size, (int)m_clients.size());

    std::unique_lock<std::recursive_mutex> lk(m_clientsMutex);
    for (auto &sock : m_clients) {
        if (cc_socket_send(&sock, data, size) != size) {
            LOG_ERR("TCP send to %s:%d failed!\n", sock.ip, sock.port);
        } 
    }

    return true;
}

const std::vector<cc_socket>* TcpServer::GetClients() 
{
    return &m_clients;
}

const cc_socket* TcpServer::GetClient(unsigned int index)
{
    if (index < m_clients.size()) {
        return &m_clients[index];
    }
    return nullptr;
}

bool TcpServer::DisconnectClient(cc_socket_handle hdl)
{
    std::unique_lock<std::recursive_mutex> lk(m_clientsMutex);

    for (unsigned int i=0; i<m_clients.size(); i++) {
        if (m_clients[i].handle == hdl) {
            LOG_INFO("Remove client %s:%d.\n", m_clients[i].ip, m_clients[i].port);
            cc_socket_close(&m_clients[i]);
            m_clients.erase(m_clients.begin() + i);
            return true; 
        }
    }
    
    return false;
}



