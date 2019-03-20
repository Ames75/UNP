#include "common.h"
using namespace std;

void usage() {
    cout << "usage: day_client [-u] [-t] <server> <port>" << endl;
}

void query_server( int sockfd ) {
    char recvline[MAXLINE + 1];
    int num = 0;
    while( (num = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[num] = 0;
        cout << recvline << endl;
    }
}

static void query_servers(vector<int>& sockfds) {
    int maxfd = 0;
    fd_set allSet;
    FD_ZERO(&allSet);
    // add all fds into set
    for ( auto sockfd : sockfds ) {
       FD_SET(sockfd, &allSet);
       if (sockfd > maxfd) maxfd = sockfd;
    }
    maxfd = maxfd + 1;
    while(true) {
        int nready = select(maxfd, &allSet, NULL, NULL, NULL); 
        if (nready == -1) {
            handle_error("query_servers");
        }
        for (auto fd:sockfds) {
            if (FD_ISSET(fd, &allSet)) {
                query_server(fd);
                nready--;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    CHECK_ARG(3);
    intStringHashMap_t arg_values;
    buildOptValMap(arg_values,argc,argv,"ut");
    vector<int> fd_socks;
    char* server = argv[optind];
    char* port = argv[optind+1];
    if(arg_values.find('u') != arg_values.end()) {
        my_udp_connect(server,port, fd_socks);
    }
    if(arg_values.find('t') != arg_values.end()) {
        my_tcp_connect(server,port, fd_socks);
    }
    cout << "here is the list of socks" << endl;
    for ( auto& fd : fd_socks) {
        my_printSockAddr(fd);
    }
    query_servers(fd_socks);
    return 0;
}
