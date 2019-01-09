#include "sys/socket.h"
#include "strings.h"
#include <netinet/in.h>
#include "common.h"

int 
main(int argc, char** argv) {
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;

    listen(listenfd, LISTENQ);
    return 0;
}
