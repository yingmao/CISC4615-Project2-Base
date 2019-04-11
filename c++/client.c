#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <pthread.h>

#define MYPORT 6666
#define BUFFER_SIZE 1024
#define Max_Message_Size 8
#define TH_FIN 1 // we need this one
#define TH_SYN 2 // we need this one
#define TH_RST 4
#define TH_PUSH 5
#define TH_ACK 6 // we need this one
#define TH_URG 7

typedef struct
{
  int th_seq;
  int th_ack;
  int th_flags;
  int th_win;
} tcphdr;

typedef struct
{
  tcphdr my_tcphdr;
  char payload[Max_Message_Size];
} packet;

#define PACKET_SIZE sizeof(packet)

// Accepts a file name and a buffer and writes the contents of the file into the buffer
// void loadFromFile(char *filename, char *buffer)
// {
//   FILE *fp;
//   char ch;
//   fp = fopen(filename, "r");
//   if (fp == NULL)
//   {
//     printf("Could not open file %s", filename);
//   }

//   while ((buffer = fgetc(fp)) != EOF)
//   {
//     printf("%s", buffer);
//   }
//   fclose(fp);
// }

void displayPacket(packet *p)
{
  printf("Packet Information\n");
  printf("Sequence #: %i\n", p->my_tcphdr.th_seq);
  printf("ACK #: %i\n", p->my_tcphdr.th_ack);
  printf("Flag: %i\n", p->my_tcphdr.th_flags);
  printf("Window Size: %i\n", p->my_tcphdr.th_win);
  printf("Payload: %s\n\n", p->payload);
}

void serialize(packet *p, char *buffer)
{

//  displayPacket(p);
  memset(buffer, 0, PACKET_SIZE);
//  displayPacket(p);
  memcpy(buffer, p, PACKET_SIZE);

}

void deserialize(packet *p, char *buffer)
{
  memset(p, 0, PACKET_SIZE);
  memcpy(p, buffer, PACKET_SIZE);
//  displayPacket(p);

//  displayBuffer(buffer);
}


int main()
{
  ///sockfd
  int sock_cli = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(MYPORT);
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("connect");
    exit(1);
  }

  // Create the initial SYN packet
  packet *initialPacket = (packet *)malloc(PACKET_SIZE);
  initialPacket->my_tcphdr.th_seq = 0;
  initialPacket->my_tcphdr.th_ack = 0;
  initialPacket->my_tcphdr.th_flags = TH_SYN;
  initialPacket->my_tcphdr.th_win = 1;

  // Serialize the initial packet
  char *buffer = (char *)malloc(PACKET_SIZE);

  packet * testPacket = (packet *) malloc(PACKET_SIZE);
  // Theory: buffer can't output anything but upon using it to create a new struct it works
  serialize(initialPacket, buffer);
  deserialize(testPacket, buffer);
  displayPacket(initialPacket);
  displayPacket(testPacket);

  // free(buffer);
  // free(initialPacket);

  // TCP Handshake
  // Send the initial SYN
  send(sock_cli, buffer, BUFFER_SIZE, 0);
  printf("Sent the SYN\n");

  // Receive the SYN ACK
  recv(sock_cli, buffer, BUFFER_SIZE, 0);
  printf("Received the SYN ACK\n");

  // Send the ACK
  send(sock_cli, buffer, BUFFER_SIZE, 0);
  printf("Sent the ACK\n");

  // Start sending the data
  printf("Sending the data now\n");
  int pos = 0;
  int len = 0;

  while (pos < PACKET_SIZE)
  {
    len = send(sock_cli, buffer + pos, BUFFER_SIZE, 0);
    if (len <= 0)
    {
      perror("ERROR");
      break;
    }
    pos += len;
  }

  printf("I sent a Type %d message\n", initialPacket->my_tcphdr.th_flags);
  free(buffer);
  free(initialPacket);

  close(sock_cli);
  printf("Send over!\n");
  return 0;
}
