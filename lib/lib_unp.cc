#include "common.h"
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

