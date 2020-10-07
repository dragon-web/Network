#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>


int main(int argc,char *argv[])
{
  if(argc != 2) // 服务器端启动参数
  {
    printf("Using :./server port\nExample :./server 8080\n\n");
    return -1;
  }
  int listened;
  if(listened = socket(AF_INET,SOCK_STREAM,0) == -1)  //创建协议
  {
    perror("socket");
    return -1;
  }
  //把服务器用于通信的地址和端口绑定到socket上
  
  struct sockaddr_in servaddr; // 服务端的地址信息的数据结构
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET; // 协议族
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
  servaddr.sin_port = htons(atoi(argv[1]));//用于指定通信端口
  if(bind(listened,(struct sockaddr*)&servaddr,sizeof(servaddr)) != 0)
  {
    perror("bind!");
    close(listened);
    return -1;
  }
  //将socket设置成监听模式
if(listen(listened,5) != 0)
{
  perror("listen");
  close(listened);
  return -1;
}

int clientfd;
int socklen = sizeof(struct sockaddr_in);
struct sockaddr_in clientaddr; // 客户端地址信息 
clientfd = accept(listened,(struct sockaddr*)&clientaddr,(socklen_t*)&socklen);
printf("客户端  (%s) 已连接。\n",inet_ntoa(clientaddr.sin_addr));

char buffer[1024];
while(1)
{
    int iret;
    memset(buffer,0,sizeof(buffer));
    if((iret = recv(clientfd,buffer,sizeof(buffer),0) ) <= 0) //接收客户端请求报文
    {
      printf("iret = %d\n",buffer);
      break;
    }
    printf("接收 :%s\n",buffer);
}
  return 0;
}
