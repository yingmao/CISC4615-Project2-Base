#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

struct struct_sample {
    char str[50];
    uint32_t num;
};

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(19990);
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    cout<<"Wating...."<<endl;
    char buff[100] = {0};
    struct struct_sample sample;
    recvfrom(sockfd, buff, sizeof(sample), 0, NULL, NULL);
    memcpy(&sample,buff,sizeof(sample));
    char num[50];
    sprintf(num,"%d",sample.num);
    cout<<"string:"<<sample.str<<endl<<"num::"<<num<<endl;
    close(sockfd);
}
