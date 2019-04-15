#include <stdio.h>   
#include <string.h>   
#include <errno.h>   
#include <stdlib.h>   
#include <unistd.h>   
#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
using namespace std;
extern int errno;

struct struct_sample {
    char str[50];
    uint32_t num;
};

int main(int argc,char* argv[])
{
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr_serv;
    memset(&addr_serv, 0, sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_serv.sin_port = htons(19990);
    struct struct_sample sample;
    memset(sample.str,0,50);
    sprintf(sample.str,"Hello,World!");
    sample.num = 1234;
    int send_num = sendto(sock_fd, &sample, sizeof(sample), 0, (struct sockaddr *)&addr_serv, sizeof(struct sockaddr_in));
    printf("Send %d bytes finished.\n",send_num);
    close(sock_fd);
}

