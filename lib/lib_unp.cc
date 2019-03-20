#include "common.h"
#include <netdb.h>

int my_bind(int sockfd, const struct sockaddr *addr,
			   socklen_t addrlen) {
	int result=0;
	if ((result=bind(sockfd, (sockaddr*)addr, addrlen)) != 0) {
		if(handle_error(std::string("bind in server"))) {
            close(sockfd);
			exit(1);
		}
	}
	return result;
}

int my_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int result = accept(sockfd, addr, addrlen);
    if( result < 0) {
        handle_error("in my_accept");
    }
    return result;
}


int my_inet_pton(int family, const char *strptr, void *addrptr) {
    if(!inet_pton(family, strptr, addrptr)) {
        std::cout<<"server ip address is wrong !"<<std::endl;
        return 1;
    }
    return 0;
};

// here src is struct sockaddr
const char* my_inet_ntop(int af, const sockaddr *src,
                             char *dst, socklen_t size) {
    if (af != AF_INET && af != AF_INET6) {
        std::cout<<"address family type "<<af<<" not supported"<<std::endl;
        return nullptr; 
    }
    const char* result;
    if( af == AF_INET) {
        result = inet_ntop(af,&(((const sockaddr_in*)src)->sin_addr),
                            dst, size);
    } else {
        result = inet_ntop(af,&(((const sockaddr_in6*)src)->sin6_addr),
                            dst, size);
    }
    
    if(result == nullptr) {
        handle_error("within my_inet_ntop");
        exit(1);
    }
    return result;
}

uint16_t my_getSockPort(const sockaddr *src) {
    int af = src->sa_family;
    if (af != AF_INET && af != AF_INET6) {
        std::cout<<"getSockPort:address family type "<<
                 af<<" not supported"<<std::endl;
        return 0; 
    }
    if (af == AF_INET) {
        return htons(((const sockaddr_in*)src)->sin_port);
    } else {
        return htons(((const sockaddr_in6*)src)->sin6_port);
    }
}

std::string my_sock_ntop(const struct sockaddr* src, socklen_t size) {
    std::string result;
    char ipString[128];
    uint16_t port;
    const char* tmp = my_inet_ntop(src->sa_family,
                             src, ipString, sizeof(ipString));
    if(tmp) result = std::string(tmp);
    port = my_getSockPort(src);
    if(port != 0) result += std::string(":")+std::to_string(port);
    return result;
}


sighandler_t my_signal(int signum, sighandler_t handler) {
    sighandler_t result;
    if((result = signal(signum, handler)) ==  SIG_ERR) {
        handle_error("in my_signal");
    }
    return result;
}

void
my_getsockname(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	if (getsockname(fd, sa, salenptr) < 0)
		handle_error("getsockname error");
}

void printAddrInfo(const struct addrinfo* info) {
    while(info != nullptr) {
        std::string res;
        if(info->ai_canonname) {
            std::cout << "cname: " << info->ai_canonname << std::endl;
        }
        res = my_sock_ntop(info->ai_addr, info->ai_addrlen);
        std::cout << res << std::endl;
        info = info->ai_next;
    }
}
struct addrinfo* host_serv(const char* host, const char* port, 
                               int family, int socktype, bool verbose) {
    struct addrinfo hints;
    struct addrinfo* result;
    bzero(&hints, sizeof(hints));
    hints.ai_flags = hints.ai_flags | AI_CANONNAME;
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    int returnNo = getaddrinfo(host, port, &hints, &result);
    if(returnNo == 0 && verbose) {
        printAddrInfo(result);
    } else if( returnNo != 0) {
        std::cout << gai_strerror(returnNo) << std::endl;
        return nullptr;
    }
    return result;
}

void my_common_connect(const char* host, const char* serv, 
                        int family, int socktype, 
                        std::vector<int>& serverSocks) 
{
    int sockfd;
    struct addrinfo *result;
    struct addrinfo* tmp;

    if( (result = host_serv(host, serv, family, socktype)) == nullptr ){
        err_sys("my_common_connect failed: ");
    }
    tmp = result;
    while ( tmp ) {
        sockfd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
        if(sockfd < 0) {
            std::cout << "can't create socket for: " << std::endl;
            printAddrInfo(tmp);
            tmp = tmp->ai_next;
            continue;
        }
        if(connect(sockfd, tmp->ai_addr, tmp->ai_addrlen) == 0) {
            serverSocks.push_back(sockfd);
        } else {
            handle_error("In my_common_connect fail to connect :");
            printAddrInfo(tmp);
            close(sockfd);
        }
        tmp = tmp->ai_next;
    }
    freeaddrinfo(result);
}

void my_udp_connect(const char* host, const char* serv, 
                    //std::vector<my_sockaddr_t>& serverAddrs
                    std::vector<int>& serverSocks) {
    int sockfd;
    struct addrinfo *result, *tmp;
    if( (result = host_serv(host, serv, AF_UNSPEC, SOCK_DGRAM)) == nullptr ){
        err_sys("my_tcp_connect failed");
    }
    tmp = result;
    while ( tmp ) {
        sockfd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
        if(sockfd < 0) {
            std::cout << "can't create socket for: " << std::endl;
            printAddrInfo(tmp);
            continue;
            tmp = tmp->ai_next;
        }
        if(connect(sockfd, tmp->ai_addr, tmp->ai_addrlen) == 0) {
            //my_sockaddr_t addr;
            //addr.m_addr = result->ai_addr;
            //addr.m_addrlen = result->ai_addrlen;
            //serverAddrs.push_back(addr);
            serverSocks.push_back(sockfd);
        } else {
            handle_error("In my_connect fail to connect :");
            printAddrInfo(tmp);
        }
        tmp = tmp->ai_next;
    }
    freeaddrinfo(result);
    return;
}

void my_tcp_connect(const char* host, const char* serv,
                        std::vector<int>& serverSocks) {
    int sockfd;
    struct addrinfo *result;
    struct addrinfo* tmp;

    if( (result = host_serv(host, serv, AF_UNSPEC, SOCK_STREAM)) == nullptr ){
        err_sys("my_tcp_connect failed");
    }
    tmp = result;
    while ( tmp ) {
        sockfd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
        if(sockfd < 0) {
            std::cout << "can't create socket for: " << std::endl;
            printAddrInfo(tmp);
            tmp = tmp->ai_next;
            continue;
        }
        if(connect(sockfd, tmp->ai_addr, tmp->ai_addrlen) == 0) {
            serverSocks.push_back(sockfd);
        } else {
            handle_error("In my_tcp_connect fail to connect :");
            printAddrInfo(tmp);
            close(sockfd);
        }
        tmp = tmp->ai_next;
    }
    freeaddrinfo(result);
}

void
my_getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	if (getpeername(fd, sa, salenptr) < 0)
		err_sys("getpeername error");
}

void my_printSockAddr(int fd) {
    struct sockaddr_storage sa;
    socklen_t len;
    if (getpeername(fd, (struct sockaddr *)&sa, &len) < 0 ) {
        perror("in my_printSockAddr");
    } else {
        std::cout << "connected to " 
                  << my_sock_ntop((const struct sockaddr*)&sa, len) 
                  << std::endl;
    }
}

int my_setsockopt(int sockfd, int level, int optname,
                      const void *optval, socklen_t optlen) {
    int code;
    if ( (code = setsockopt(sockfd, level, optname, optval, optlen)) != 0) { 
        std::cout << "optname" << optname << std::endl;
        handle_error("in setsockopt");
    }
    return code;
}


void my_tcp_listen(const char* host, const char* port,
                         std::vector<int>& listenfds) {
    struct addrinfo hints;
    struct addrinfo* result;
    int code = 1;
    bzero(&hints, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    int returnNo = getaddrinfo(host,port,&hints,&result);
    if( returnNo != 0) {
        std::cout << "getaddrinfo failed due to " 
                    << gai_strerror(returnNo) << std::endl;
        exit(1);
    }
    struct addrinfo* tmp = result;
    while(tmp != nullptr)  {
        int listenfd = socket(tmp->ai_family, 
                              tmp->ai_socktype, tmp->ai_protocol);
        my_setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR, &code,sizeof(code));
        my_setsockopt(listenfd,SOL_SOCKET,SO_REUSEPORT, &code,sizeof(code));
        if(listenfd < 0) {
            handle_error("listenfd < 0");
            printAddrInfo(tmp);
            std::cout << "try next addrinfo" << std::endl;
            continue;
        }
        if ( my_bind(listenfd,tmp->ai_addr,tmp->ai_addrlen) == 0) {
            listen(listenfd, LISTENQ);
            listenfds.push_back(listenfd);
        } 
        tmp = tmp->ai_next;
    }
    freeaddrinfo(result);
}


