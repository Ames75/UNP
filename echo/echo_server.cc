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
static void process_echo(int connfd) {
    ssize_t n;
    char buf[MAXLINE];
    again:
    while((n = read(connfd, buf, MAXLINE)) > 0 ) {
        process_str(buf, n);
        sleep(3);
        write(connfd, buf, n);
    }
    if(n < 0 && errno == EINTR) {
        goto again;
    } else if ( n < 0 ){
        err_sys("str_echo: read error");
    }
}

static run_service(int listenfd) {
    struct sockaddr_t cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    set<int> connfdSet;
    int connfd, nready;
    int maxfd1 = listenfd + 1;
    fd_set fdSet;
    FD_ZERO(&fdSet);
    FD_SET(listenfd, &fdSet);
    if ((nready = select(maxfd1, &fdSet, NULL, NULL, NULL)) == -1 ) {
        handle_error("select in run_service");
    }
    if ((connfd = accept4(listenfd,(SA*)&cliaddr, &client)) == -1) {
        handle_error("accept4 ");
    }
    
}


int 
main(int argc, char** argv) {
    int listenfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

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
    while(true) {

    }
    return 0;
}
