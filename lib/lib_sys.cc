#include "common.h"

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

