#include "common.h"
using namespace std;

void usage() {
    cout << "day_server <port> " << endl;
}


static void insert2fdArray(struct pollfd fdArray[], 
                             size_t aSize, int aFD) {
    size_t i;
    for ( i = 0; i < aSize; i++ ) {
        if(fdArray[i].fd < 0) {
            fdArray[i].fd = aFD;
            fdArray[i].events = POLLRDNORM;
            break;
        }
    }
    if ( i == aSize) {
        cout << "too many connections" << endl;
    }
    return;
}

void run_service_poll(const char* service) {
    struct pollfd connfds[FOPEN_MAX];
    vector<int> listenfds;
    my_tcp_listen(nullptr, service, listenfds);
    struct pollfd* polllistens = new pollfd[listenfds.size()];
    int connfd;
    struct sockaddr_storage cliaddr;
    socklen_t len = sizeof(cliaddr);
    for( size_t i = 0; i < listenfds.size(); i++) {
        polllistens[i].fd = listenfds[i];
        polllistens[i].events = POLLRDNORM;
    }
    for (int i = 0; i < FOPEN_MAX; i++) {
        connfds[i].fd = -1;
    }
    while(true) {
        // first poll listen fds
        int readyl = poll(polllistens, listenfds.size(), INFTIM);
        for (size_t i = 0; i < listenfds.size(); i++) {
            if (readyl <= 0) break;
            if (polllistens[i].revents & POLLRDNORM) {
                connfd = accept(polllistens[i].fd, 
                                (struct sockaddr*) &cliaddr,
                                &len);
                cout << "a new client from " 
                     << my_sock_ntop((struct sockaddr*) &cliaddr, len) << endl;
                if (connfd > 0) {
                    insert2fdArray(connfds, FOPEN_MAX, connfd);
                }
                readyl--;
            }
        }
        // now poll connfd;
        int readycon = poll(connfds, FOPEN_MAX, 1000);
        for( int i = 0; i < FOPEN_MAX; i++) {
            if(connfds[i].fd < 0) continue;
            if(connfds[i].revents & (POLLRDNORM | POLLERR)) {
                ;
            }
        }
    }
    delete[] polllistens;
    return;
}

int main(int argc, char* argv[]) {
    CHECK_ARG(2);
    run_service_poll("daytime");
    return 0;
}
