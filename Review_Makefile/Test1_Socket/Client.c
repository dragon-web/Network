#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#define SERV_PORT 6666
#define SERV_IP "172.16.0.12"

int main()
{
  int cfd;
  int n;
  cfd = socket(AF_INET,SOCK_STREAM,0);
  char buf[BUFSIZ];
  struct sockaddr_in serv_addr;

//socklen_t serv_addr_len;
  cfd = socket(AF_INET,SOCK_STREAM,0);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port =htons(SERV_PORT);
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  connect(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
  while(1)
  {
  fgets(buf,sizeof(buf),stdin);
  write(cfd,buf,strlen(buf));
  n =  read(cfd,buf,sizeof(buf));
  write(STDOUT_FILENO,buf,n);
  }
  close(cfd);

  return 0;
}
