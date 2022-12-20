#ifndef NETWORK_H_ 
#define NETWORK_H_

#include <vector>
#include <stdint.h>

typedef int cc_socket_handle;

enum cc_socketerr
{
    CC_SOCKET_OK = 0,
    CC_SOCKET_ERR,
};

struct cc_socket
{
    cc_socket_handle handle;
    char ip[32];
    unsigned short port;
};

cc_socketerr cc_socket_init(cc_socket *s, bool btcp = true);

cc_socketerr cc_socket_connect(cc_socket *s, const char* ip, unsigned short port, uint32_t timeoutMs = 3000);

bool cc_socket_is_connect(cc_socket *s);

cc_socketerr cc_socket_getsockopt(cc_socket *s, int level, int optname, char* optval, int* optlen);

cc_socketerr cc_socket_setsockopt(cc_socket *s, int level, int optname, const char* optval, int optlen);

cc_socketerr cc_socket_close(cc_socket *s);

cc_socketerr cc_socket_bind(cc_socket *s, unsigned short port);

cc_socketerr cc_socket_listen(cc_socket *s, int maxnum);

cc_socketerr cc_socket_accept(cc_socket *s, cc_socket *clt);

int cc_socket_select(cc_socket *s, const std::vector<cc_socket> *readSet, 
    std::vector<cc_socket> *setted, int tmoMs);

int cc_socket_send(cc_socket *s, const char* buf, unsigned int size);

int cc_socket_recv(cc_socket *s, char* buf, unsigned int size);

int cc_socket_send_to(cc_socket *s, const char* buf, unsigned int size, cc_socket *to);

int cc_socket_recv_from(cc_socket *s, char* buf, unsigned int size, cc_socket *clt);

cc_socketerr cc_socket_add_membership(cc_socket *s, const char* groupip, int loopback);

cc_socketerr cc_socket_drop_membership(cc_socket *s, const char* groupip);

void cc_socket_set_recv_snd_buf(cc_socket *s, int recv, int send);

unsigned long cc_inet_addr(const char* cp);

unsigned long cc_htonl(unsigned long hostlong);
unsigned long cc_ntohl(unsigned long netlong);
unsigned short cc_htons(unsigned short hostshort);
unsigned short cc_ntohs(unsigned short netshort);
unsigned long long cc_htonll(unsigned long long hostll);
unsigned long long cc_ntohll(unsigned long long netll);

#endif


