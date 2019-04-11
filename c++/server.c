#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HELLO_WORLD_SERVER_PORT 6666
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024

#define Max_Message_Size 8
#define TH_FIN 1 // we need this one
#define TH_SYN 2 // we need this one
#define TH_RST 4
#define TH_PUSH 5
#define TH_ACK 6 // we need this one
#define TH_URG 7

#define window_size 4 // This is temporary

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
} message;

typedef struct
{
  tcphdr my_tcphdr;
  char payload[window_size];
} packet;

int main(int argc, char **argv)
{
  // set socket's address information
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htons(INADDR_ANY);
  server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);

  // create a stream socket
  int server_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (server_socket < 0)
  {
    printf("Create Socket Failed!\n");
    exit(1);
  }

  //bind
  if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)))
  {
    printf("Server Bind Port: %d Failed!\n", HELLO_WORLD_SERVER_PORT);
    exit(1);
  }

  // listen
  if (listen(server_socket, LENGTH_OF_LISTEN_QUEUE))
  {
    printf("Server Listen Failed!\n");
    exit(1);
  }

  while (1)
  {
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    int new_server_socket = accept(server_socket, (struct sockaddr *)&client_addr, &length);
    if (new_server_socket < 0)
    {
      printf("Server Accept Failed!\n");
      break;
    }

    // TCP Handshake

    // Allocate memory to deserialize the packet
    packet *initialPacket = (packet *)malloc(sizeof(packet));
    int needRecv = sizeof(packet);
    char *buffer = (char *)malloc(needRecv);

    // Receive the initial SYN
    recv(new_server_socket, buffer, BUFFER_SIZE, 0);
    printf("Received the SYN\n");

    // add 1 to the received sequence number
    // Send the SYN ACK
    send(new_server_socket, buffer, BUFFER_SIZE, 0);
    printf("Sent the SYN ACK\n");

    // Receive the ACK
    recv(new_server_socket, buffer, BUFFER_SIZE, 0);
    printf("Received the ACK\n");

    // Start receiving the data
    printf("Receiving data now\n");
    int pos = 0;
    int len;

    while (pos < needRecv)
    {
      len = recv(new_server_socket, buffer + pos, BUFFER_SIZE, 0);

      printf("len = %d\n", len);

      if (len < 0)
      {
        printf("Server Recieve Data Failed!\n");
        break;
      }
      pos += len;
    }

    close(new_server_socket);
    memcpy(initialPacket, buffer, needRecv);

    printf("This is a Type %d message\n", initialPacket->my_tcphdr.th_flags);
    printf("Message: %s\n", initialPacket->payload);

    free(buffer);
    free(initialPacket);
  }

  close(server_socket);
  return 0;
}
