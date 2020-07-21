#define MAXBUF 1024
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>

int main(int argc,char* argv[])
{
  int pid;
  int sockfd,new_fd;
  socklen_t len;
  struct sockaddr_in my_addr,their_addr;
  unsigned int myport,lisnum;
  char bud[MAXBUF + 1];
  if(argv[2])
  {
    myport = atoi(argv[2])
  }
  else{
    myport = 6379;
  }



}
