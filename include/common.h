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

void err_sys(const char* x) 
{ 
    perror(x); 
    exit(1); 
}

// return true if we should exit the program
bool handle_error(const std::string& place) {
	std::cout<<place<<" failure due to " << strerror(errno) << std::endl;
	return false;
}

// return true if we should exit the program
bool handle_error(const char* place) {
	std::cout<<place<<" failure due to " << strerror(errno) << std::endl;
	return false;
}

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
        result = inet_ntop(af,&((const sockaddr_in*)src)->sin_addr,dst, size);
    } else {
        result = inet_ntop(af,&((const sockaddr_in6*)src)->sin6_addr,dst, size);
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


#endif
