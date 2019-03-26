#include "common.h"

void err_sys(const char* x) 
{ 
    if(isDaemon) {
        syslog(LOG_ERR | LOG_LOCAL0, 
               "System quit due to %s",
               strerror(errno));
    } else perror(x); 
    exit(1); 
}

void printInfo(const std::string& msg) {
    if(isDaemon) {
        syslog
    }

}

// return true if we should exit the program
bool handle_error(const std::string& place) {
    if(isDaemon) {
        syslog(LOG_ERR | LOG_LOCAL0, 
               "handle error %s at %s",
               strerror(errno), place.c_str());
    } else {
        std::cout<<place<<" failure due to " << strerror(errno) << std::endl;
    }
	return false;
}

// return true if we should exit the program
bool handle_error(const char* place) {
    if(isDaemon) {
        syslog(LOG_ERR | LOG_LOCAL0, 
               "handle error %s at %s",
               strerror(errno), place);
    } else {
        std::cout<<place<<" failure due to " << strerror(errno) << std::endl;
    }

	return false;
}

