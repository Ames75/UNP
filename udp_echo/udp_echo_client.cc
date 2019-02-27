#include "common.h"

using namespace std;
void usage() {
    cout<<"Usage: udp_echo_client <ServerIp>" <<endl;
}

static void udp_echo_client_proc(int sockfd, const sockaddr* servaddr,
                                         socklen_t servlen) 
{
    int n;
    char sendbuffer[MAXLINE], recvbuffer[ MAXLINE + 1];
    char fromIP[128];
    struct sockaddr* fromAddr = (sockaddr*)malloc(servlen);
    socklen_t len = servlen;
    while(fgets(sendbuffer, MAXLINE, stdin) != nullptr) {
        sendto(sockfd, sendbuffer, strlen(sendbuffer), 0, servaddr, servlen);
        n = recvfrom(sockfd, recvbuffer, MAXLINE, 0, 
                     (sockaddr*)fromAddr, &len);
        recvbuffer[n] = 0;
        fputs("I've received a message ",stderr);
        fputs(recvbuffer, stderr);
        if(my_inet_ntop(fromAddr->sa_family,fromAddr,fromIP,sizeof(fromIP))) {
            cout << "it is from " << fromIP 
                 << " port " << my_getSockPort(fromAddr) << endl;
        }      
    }
    free(fromAddr);
}

static void udp_echo_client_connect(int sockfd, const sockaddr* servaddr,
                                         socklen_t servlen) 
{
    int n;
    char sendbuffer[MAXLINE], recvbuffer[ MAXLINE + 1];
    char fromIP[128];
    connect(sockfd, const_cast<sockaddr*>(servaddr), servlen);
    // display local address
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    my_getsockname(sockfd,(struct sockaddr *) &cliaddr, &len);
    cout << "local address is " << my_sock_ntop(
                                    (struct sockaddr *)&cliaddr, len) << endl;
    while(fgets(sendbuffer, MAXLINE, stdin) != nullptr) {
        write(sockfd, sendbuffer, strlen(sendbuffer));
        n = read(sockfd, recvbuffer, MAXLINE);
        if( n < 0 ) {
            handle_error("after read in udp_echo_client_connect");
            continue;
        }
        recvbuffer[n] = 0;
        fputs("I've received a message ",stderr);
        fputs(recvbuffer, stderr);
        if(my_inet_ntop(servaddr->sa_family,servaddr,fromIP,sizeof(fromIP))) {
            cout << "it is from " << fromIP 
                 << " port " << my_getSockPort(servaddr) << endl;
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
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(ECHO_PORT);
    if(my_inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) {
       return 1;
    }
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // udp_echo_client_proc(sockfd,(sockaddr*)&servaddr,sizeof(servaddr));
    udp_echo_client_connect(sockfd,(sockaddr*)&servaddr,sizeof(servaddr));
    return 0;
}
