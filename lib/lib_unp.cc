#include "common.h"
#include <netdb.h>

int my_bind(int sockfd, const struct sockaddr *addr,
			   socklen_t addrlen) {
	int result=0;
	if ((result=bind(sockfd, (sockaddr*)addr, addrlen)) != 0) {
		if(handle_error(std::string("bind in server"))) {
			exit(1);
		}
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
    } else {
        std::cout << gai_strerror(returnNo) << std::endl;
        return nullptr;
    }
    return result;
}

int my_tcp_connect(const char* host, const char* serv) {
    int sockfd;
    struct addrinfo *result;
    struct addrinfo* tmp;

    if( (result = host_serv(host, serv, AF_UNSPEC, SOCK_STREAM)) == nullptr ){
        err_sys("my_tcp_connect failed");
    }
    while ( tmp ) {
        sockfd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
        if(sockfd < 0) {
            std::cout << "can't create socket for: " << std::endl;
            printAddrInfo(tmp);
            continue;
        }
        if(connect(sockfd, tmp->ai_addr, tmp->ai_addrlen) == 0) {
            break;
        } else {
            handle_error("In my_tcp_connect fail to connect :");
            printAddrInfo(tmp);
        }
        close(sockfd);
        tmp = tmp->ai_next;
    }
    if( !tmp ) {
        std::cout << " no connection established" << std::endl;
        sockfd = -1;
    }
    freeaddrinfo(result);
    return sockfd;
}



