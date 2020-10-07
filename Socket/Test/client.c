#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>


int main(int argc ,char *argv[])
{

  if(argc != 3)
  {
    printf("Using :./client ip port\nExample :./client 127.0.0.1 8080\n\n");
    return -1;
  }
 // 创建客户端参数 
int sockfd;
  if(sockfd = socket(AF_INET,SOCK_STREAM,0) == -1)
  {
    perror("socket");
    return -1;
  }
  struct hostent *h;
  if(h = gethostbyname(argv[1]) == 0)
  {
    printf("gethostbyname failed.\n");
    close(sockfd);
    return -1;
  }
struct sockaddr_in servaddr;
memset(&servaddr,0,sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_port = htons(atoi(argv[2]));
memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);

if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) != 0)  //向服务端发起连接请求
{
  perror("connect");
  close(sockfd);
  return -1;
}

char buffer[1024];
for(int i = 0; i < 3;++i)
{
  int iret;
  memset(buffer,0,sizeof())

}

  return 0;
}
