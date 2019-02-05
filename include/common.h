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


#endif
