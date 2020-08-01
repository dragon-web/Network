#pragma once

#include<iostream>
#include"Sock.hpp"
#include"Log.hpp"
#include"Protocol.hpp"

#define DefaultPort 8081

class HttpServer
{
	public:
		HttpServer(int _port = DefaultPort):listen_sock(-1),port(_port)
		{}
		~HttpServer()
		{
			if(listen_sock >= 0)
			{
				close(listen_sock);
			}
		}
	public:
		void InitServer()
		{
			listen_sock = Sock::Socket();
			Sock::Bind(listen_sock,port);
			Sock::Listen(listen_sock);
		}
		void Start()
		{
			LOG(Normal,"server start success...");
			for(;;)
			{
				int sock = Sock::Accept(listen_sock);
				if(sock >= 0)
				{
					LOG(Normal,"get a new linking...");
					pthread_t tid;
					int *p = new int(sock);
					pthread_create(&tid,nullptr,Entry::HandlerRequest,(void *)p);
				}
			}
		}
	private:
		int listen_sock;
		int port;
};
