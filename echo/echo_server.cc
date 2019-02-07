#include "sys/socket.h"
#include "strings.h"
#include <netinet/in.h>
#include "common.h"
#include <string>
#include <ctype.h>


using namespace std;

static inline void process_str(char* buf, ssize_t length) {
    for(int i=0; i<length; i++) {
        buf[i] = tolower(buf[i]);
    }
}

// return true if client closes
static bool process_echo(int connfd) {
    ssize_t n;
    char buf[MAXLINE];
    again:
    while((n = read(connfd, buf, MAXLINE)) > 0 ) {
        process_str(buf, n);
        write(connfd, buf, n);
    }
    if(n < 0 && errno == EINTR) {
        goto again;
    } else if ( n < 0 ){
        err_sys("str_echo: read error");
    }
    if (n == 0) {
       close(connfd);
       return true;  
    }
    return false;
}

static void run_service(int listenfd) {
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    set<int> connfdSet;
    int connfd, nready;
    int maxfd1 = listenfd + 1;
    fd_set fdSet;
    FD_ZERO(&fdSet);
    FD_SET(listenfd, &fdSet);
    while(true) {
     if ((nready = select(maxfd1, &fdSet, NULL, NULL, NULL)) == -1 ) {
        handle_error("select in run_service");
     }
    // if there is a new connection
    if (FD_ISSET(listenfd, &fdSet)) { 
    	if ((connfd = accept4(listenfd,(sockaddr*)&cliaddr, &clilen, 0)) == -1) {
        	handle_error("accept4 ");
    	} 
        connfdSet.insert(connfd);
        if( connfdSet.size() == (FD_SETSIZE - 3)) {
	}  
        // add new connfd into the set
        FD_SET(connfd, &fdSet);
        maxfd1 = maxfd1>(connfd+1)?maxfd1:(connfd+1);  
        nready--;
    }
    for(auto readyClient : connfdSet) {
       if (nready == 0) break;
       if(FD_ISSET(readyClient, &fdSet)) {
           if ( process_echo(readyClient) ) {
           	connfdSet.erase(readyClient);
                FD_CLR(readyClient, &fdSet); 
           }
           nready--; 
       }    
    }
  } 
}


int 
main(int argc, char** argv) {
    int listenfd;
    struct sockaddr_in servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(ECHO_PORT);

    if (bind(listenfd, (sockaddr*)(&servaddr), sizeof(servaddr)) != 0) {
        if(handle_error(string("bind in server"))) {
            exit(1);
        }
    }
    listen(listenfd, LISTENQ);
    run_service(listenfd);  
    return 0;
}
