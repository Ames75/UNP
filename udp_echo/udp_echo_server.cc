#include "sys/socket.h"
#include "strings.h"
#include <netinet/in.h>
#include "common.h"
#include <string>
#include <ctype.h>
using namespace std;
static size_t packet_count;

static inline void process_str(char* buf, ssize_t length) {
    for(int i=0; i<length; i++) {
        buf[i] = tolower(buf[i]);
    }
}

static void 
server_SIGINT_handler(int signo) {
    cout<<"So far,"<<packet_count<<" packets processed"<<endl;
    exit(0);
}

void udp_echo_service (int sockfd, sockaddr* cliaddr, socklen_t clilen) {
    int num_recv =  0;
    char mesg[MAXLINE];
    char fromIP[128];
    if(my_signal(SIGINT,server_SIGINT_handler) == SIG_ERR) {
       exit(1);
    }
    while(true) {
        socklen_t len=clilen;
        num_recv = recvfrom( sockfd, mesg, MAXLINE, 0, cliaddr, &len);
        process_str(mesg, num_recv);
        cout<<"msg received from "<< 
            std::string(my_inet_ntop(cliaddr->sa_family, cliaddr, fromIP, 128))
            <<" port " << my_getSockPort(cliaddr) << endl;
        sendto(sockfd, mesg, num_recv, 0, cliaddr, len);
        packet_count++;
    }
}


int 
main(int argc, char** argv) {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(ECHO_PORT);

    my_bind(sockfd, (sockaddr *)&servaddr, sizeof(servaddr));
    udp_echo_service(sockfd, (sockaddr*)&cliaddr, sizeof(cliaddr));
    return 0;
}

