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
    struct sockaddr_in fromAddr;
    socklen_t len = servlen;
    while(fgets(sendbuffer, MAXLINE, stdin) != nullptr) {
        sendto(sockfd, sendbuffer, strlen(sendbuffer), 0, servaddr, servlen);
        n = recvfrom(sockfd, recvbuffer, MAXLINE, 0, 
                     (sockaddr*)&fromAddr, &len);
        recvbuffer[n] = 0;
        fputs("I've received a message ",stderr);
        fputs(recvbuffer, stderr);
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
    udp_echo_client_proc(sockfd,(sockaddr*)&servaddr,sizeof(servaddr));
    return 0;
}
