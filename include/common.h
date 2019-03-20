#ifndef COMMON_H
#define COMMON_H
#include <time.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <set>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <getopt.h>
#include <vector>
#include <netdb.h>
#include <unordered_map>

/* Posix.1g requires that an #include of <poll.h> DefinE INFTIM, but many
   systems still DefinE it in <sys/stropts.h>.  We don't want to include
   all the streams stuff if it's not needed, so we just DefinE INFTIM here.
   This is the standard value, but there's no guarantee it is -1. */
#ifndef INFTIM
#define INFTIM          (-1)    /* infinite poll timeout */
#endif
//#include <sys/select.h>


#define MAXLINE 4096
#define LISTENQ 256
#define ECHO_PORT 1313

#define CHECK_ARG(num)  if ( argc < num ) { usage(); exit(1); }

typedef struct {
    sockaddr* m_addr;
    socklen_t m_addrlen;
} my_sockaddr_t;

using intStringHashMap_t = std::unordered_map<int, std::string>;

void err_sys(const char* x);
// return true if we should exit the program
bool handle_error(const std::string& place) ;

// return true if we should exit the program
bool handle_error(const char* place);

int my_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int my_inet_pton(int family, const char *strptr, void *addrptr);

// here src is struct sockaddr
const char* my_inet_ntop(int af, const sockaddr *src,
                             char *dst, socklen_t size);

uint16_t my_getSockPort(const sockaddr *src);

std::string my_sock_ntop(const struct sockaddr* src, socklen_t size);

sighandler_t my_signal(int signum, sighandler_t handler);

void
my_getsockname(int fd, struct sockaddr *sa, socklen_t *salenptr);

void tokenizeString(const std::string& src, 
                          const char* delimiter, 
                          std::vector<std::string>& result);

void printAddrInfo(const struct addrinfo* info);

struct addrinfo* host_serv(const char* host, const char* port, 
                           int family, int socktype, bool verbose = false);

// return successfully connected socket
void my_tcp_connect(const char* host, const char* serv,
                        std::vector<int>& serverSocks);
void my_udp_connect(const char* host, const char* serv, 
                    std::vector<int>& serverAddrs);

void my_common_connect(const char* host, const char* serv, 
                        int family, int socktype, 
                        std::vector<int>& serverSocks);

int my_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void
my_getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr);

void my_tcp_listen(const char* host, const char* port, 
                        std::vector<int>& listenfds);

void printOptValMap(const intStringHashMap_t& optMap);

size_t buildOptValMap(intStringHashMap_t& arg_values, 
                            int argc, char** argv,
                            const char* optstr);

void my_printSockAddr(int fd);


#endif
