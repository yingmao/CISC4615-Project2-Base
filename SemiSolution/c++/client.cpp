#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<cstring>
using namespace std;

struct struct_sample {
    char str[50];
    uint32_t num;
};

void send(char c,int num)
{
    struct struct_sample sample;
    sample.num = num;
    sprintf(sample.str,"%c",c);
    struct sockaddr_in addr_client;
    memset(&addr_client, 0, sizeof(addr_client));
    addr_client.sin_family = AF_INET;
    addr_client.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_client.sin_port = htons(19990);
    int sockfd_client = socket(AF_INET, SOCK_DGRAM, 0);
    sendto(sockfd_client, &sample, sizeof(sample), 0, (struct sockaddr *)&addr_client, sizeof(struct sockaddr_in));
    close(sockfd_client);
    cout<<"Send:"<<c<<",num:"<<num<<endl;
}

void recv(char* c,int* num)
{
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(19991);
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    cout<<"Wating...."<<endl;
    char buff[100] = {0};
    struct struct_sample sample;
    recvfrom(sockfd, buff, sizeof(sample), 0, NULL, NULL);
    memcpy(&sample,buff,sizeof(sample));
    cout<<"Receive:"<<sample.str<<",num:"<<sample.num<<endl;
    *c = sample.str[0];
    *num = sample.num;
    close(sockfd);
}

int main(int argc, char **argv)
{
    int num = 1;
    char c = 'a';
    while(true)
    {
        send(c,num);
        recv(&c,&num);
        if(c == 'd')
            break;
        c = c + 1;
        num++;
    }
}

