#include "common.h"
using namespace std;

void usage() {
    cout << "usage: day_client <server> <port>" << endl;
}

void query_server( int sockfd ) {
    char recvline[MAXLINE + 1];
    int num = 0;
    while( (num = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[num] = 0;
        cout << recvline << endl;
    }
}

int main(int argc, char* argv[]) {
    CHECK_ARG(3);
    int sockfd;
    struct sockaddr_storage ss;
    socklen_t len = sizeof(ss);
    sockfd = my_tcp_connect(argv[1],argv[2]);
    if (sockfd < 0) return 1;
    my_getpeername(sockfd ,(struct sockaddr*)&ss,&len);
    cout << "connected to " << my_sock_ntop((const struct sockaddr*)&ss, len) 
        << endl;
    query_server(sockfd);
    return 0;
}
