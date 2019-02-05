#include "common.h"
#include <iostream>
#include <arpa/inet.h>
using namespace std;
void usage() {
    cout<<"Usage: echo_client <ServerIp>" <<endl;
}

static void echo_client_process(int sockfd) {
    char sendline[MAXLINE], recvline[MAXLINE];
    int maxfdp1;
    fd_set readSet;
    bool clientStop = false;
    FD_ZERO(&readSet);
    while(true) {
        FD_SET(sockfd, &readSet);
        if(!clientStop) {
            FD_SET(fileno(stdin), &readSet);
        }
        maxfdp1 = max(fileno(stdin), sockfd) + 1;
        if (select(maxfdp1, &readSet, NULL, NULL, NULL) == -1 ) {
            if(handle_error(std::string("select"))){
                exit(1);
            }
        }
        // now see if socket is ready
        if(FD_ISSET(sockfd,&readSet)) {
            // now receive from the server
            int numRead = read(sockfd, recvline, MAXLINE);
            if( numRead == 0 ) {
               if (clientStop) {
                 cout << "client stopped" << endl;
               } else cout<<" server is down" << endl;
               return;
            } else if (numRead == -1) {
               handle_error(std::string("from read"));
               return;
            }
            fputs("I've received a message ",stderr);
            fputs(recvline, stderr);
        }

        if(FD_ISSET(fileno(stdin),&readSet)) {
           if(fgets(sendline, MAXLINE, stdin) != nullptr) {
               if(strcmp(sendline, "quit\n") == 0) {
                  // we should shut down connection
                  if( shutdown(sockfd, SHUT_WR) == -1) {
                     handle_error(string("shutdown"));
                  } else {
                     FD_CLR(fileno(stdin), &readSet);
                     clientStop = true;
                  }
               }
               else write(sockfd, sendline, strlen(sendline));
           } 
        }
    }
}    


int main(int argc, char** argv) {
    int sockfd;
    struct sockaddr_in servaddr;

    if(argc != 2) {
        usage();
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if(!inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) {
        cout<<"server ip address is wrong !"<<endl;
        return 1;
    };
    servaddr.sin_port = htons(ECHO_PORT);

    if(connect(sockfd, (struct sockaddr*)(&servaddr), sizeof(servaddr)) != 0) {
        cout<<"connect failure due to " << strerror(errno) << endl;
        
    };

    echo_client_process(sockfd);
    
}
