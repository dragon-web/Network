#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#define SERV_PORT 8888
//172.16.0.12 

int main(void)
{ 
  pid_t pid;
  char buf[BUFSIZ];
  int n,i;
 int lfd,cfd;
 struct sockaddr_in serv_addr,clie_addr;
 socklen_t clie_addr_len;
 lfd = socket(AF_INET,SOCK_STREAM,0);

 bzero(&serv_addr,sizeof(serv_addr));
 serv_addr.sin_family = AF_INET;
 serv_addr.sin_port = htons(SERV_PORT);
 serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
// inet_pton(AF_INET,"172.16.0.12",&serv_addr.sin_addr.s_addr);
  bind(lfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

  listen(lfd,128);
  clie_addr_len = sizeof(clie_addr);
  cfd = accept(lfd,(struct sockaddr *)&clie_addr, &clie_addr_len);
   
  pid = fork();
  if(pid < 0)
  {
   perror("fork error");
   exit(1);
  }
  else if(pid == 0)
  {
    close(lfd);
    while(1)
    {
     n =  read(cfd,buf,sizeof(buf));
     if(n == 0)//client close
     {
      close(cfd);
     }
     else if(n == -1)
     {
        perror("read error");
        exit(1);
     }

    }



  }


  return 0;
}
