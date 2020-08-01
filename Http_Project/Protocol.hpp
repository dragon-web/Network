#pragma once

#include<iostream>
#include<unordered_map>
#include<string>
#include<sstream>
#include<string.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include"Sock.hpp"
#include"Util.hpp"

#define WWWROOT "./wwwroot"
#define WELCOME_PAGE "index.html"

class HttpRequest
{
	public:
		HttpRequest():blank("\n"),path(WWWROOT),cgi(false),file_size(0),fd(-1)
		{}
		~HttpRequest()
		{
			if(fd >= 0)
				close(fd);
		}
	public:
		void SetRequestLine(std::string &line)
		{
			request_line = line;
		}
		void SetRequestHeader(std::string &header)
		{
			request_header = header;
		}
		void SetRequestBody(std::string &body)
		{
			request_body = body;
		}
		void SetUrlToPath()
		{
			path += url;
		}
		int GetFileSize()
		{
			return file_size;
		}
		//GET /index.html HTTP/1.0\n
		void UrlParse()
		{
			std::size_t pos = url.find('?');
			if(pos == std::string::npos)
			{
				path += url;
			}
			else
			{
				path += url.substr(0,pos);
				query_string = url.substr(pos + 1);
				cgi = true;
			}
		}
		void RequestLineParse()
		{
			std::stringstream ss(request_line);
			ss>>method>>url>>version;
			std::cout<<"Method : "<<method<<std::endl;
			std::cout<<"Url : "<<url<<std::endl;
			std::cout<<"Version : "<<version<<std::endl;
			if(url == "/")
			{
				url += WELCOME_PAGE;
			}

		}
		void RequestHeaderParse()
		{
			std::size_t pos = request_header.find('\n');
			int start = 0;
			while(pos != std::string::npos)
			{
				std::string sub = request_header.substr(start,pos - start);
				//sub->kv
				Util::MakeKV(header_kv,sub);//Content-Length: 30
				start = pos + 1;
				pos = request_header.find('\n',pos + 1);
			}
		}
		bool OpenResources()
		{
			bool ret = true;
			fd = open(path.c_str(),O_RDONLY);
			if(fd < 0)
			{
				LOG(Error,"open resources failed!");
				ret = false;
			}
			return ret;
		}
		int GetFd()
		{
			return fd;
		}
		std::string &GetSuffix()
		{
			return suffix;
		}
		bool IsMethodOk()
		{
			//Get gEt GET GEt
			if(strcasecmp(method.c_str(),"GET") == 0||strcasecmp(method.c_str(),"POST") == 0)
			{
				return true;
			}
			return false;
		}
		bool PathIsLegal()
		{
			bool ret = true;
			struct stat st;
			if(stat(path.c_str(),&st) == 0)
			{
				//exist
				//idr?
				if(S_ISDIR(st.st_mode))
				{
					if(path[path.size() - 1] != '/')
					{
						path += "/";
					}
					path += WELCOME_PAGE;
				}
				else if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
				{
					//可执行程序  CGI
					cgi = true;
				}
				else
				{
					//ok
				}
				file_size = st.st_size;
				std::cout<<"GetFileSize: "<<file_size<<std::endl;
				std::size_t pos = path.rfind(".");
				if(pos == std::string::npos)
				{
					suffix = ".html";
				}
				else
				{
					suffix = path.substr(pos);
				}
			}
			else
			{
				//not exist
				//404
				ret = false;
			}
			return ret;
		}
		bool IsGet()
		{
			return strcasecmp(method.c_str(),"GET") == 0;
		}
		bool IsPost()
		{
			return strcasecmp(method.c_str(),"POST") == 0;
		}
		bool IsCGI()
		{
			return cgi;
		}
		void SetCGI()
		{
			cgi = true;
		}
		int GetContentLength()
		{
			//std::unordered_map<std::string,std::string> header_kv;
			auto it = header_kv.find("Content-Length");
			if(it == header_kv.end())
			{
				LOG(Warning,"POST Request,but no request body!");
				return NORMAL_ERROR;
			}
			return Util::StringToInt(it->second);
		}
		void Show()
		{
		std::cout<<"##############start#####################"<<std::endl;
		std::cout<<"debug : " <<request_line<<std::endl;
		std::cout<<"debug : " <<request_header<<std::endl;
		std::cout<<"debug : "<< blank<<std::endl;
		std::cout<<"debug : " <<request_body<<std::endl;
		std::cout<<"debug : "<< method<<std::endl;
		std::cout<<"debug : "<< url<<std::endl;
		std::cout<<"debug : "<< version<<std::endl;


		std::cout<<"debug : "<< path<<std::endl;
		std::cout<<"debug : "<< query_string<<std::endl;
		std::cout<<"#############end #####################"<<std::endl;
		}
	private:
		std::string request_line;
		std::string request_header;
		std::string blank;
		std::string request_body;
	private:
		std::string method;
		std::string url;
		std::string version;

		std::unordered_map<std::string,std::string> header_kv;

		std::string path;
		int fd;
		std::string query_string;

		bool cgi;
		int file_size;
		std::string suffix;
};


class HttpResponse
{
	public:
		HttpResponse():blank("\n")
		{}
		~HttpResponse()
		{}
	public:
		std::string & GetResponseLine()
		{
			return response_line;
		}
		std::string & GetResponseHeader()
		{
			return response_header;
		}
		std::string & GetBlank()
		{
			return blank;
		}
	public:
		void SetResponseLine(std::string &line)
		{
			response_line = line;
		}
		void AddResponseHeader(std::string &line)
		{
			if(response_header.empty())
			{
				response_header = line;
			}
			else
			{
				response_header += line;
			}
		}
	private:
		std::string response_line;
		std::string response_header;
		std::string blank;
		std::string response_body;
};


class Connect
{
	public:
		Connect(int _sock):sock(_sock)
		{}
		~Connect()
		{
			if(sock >= 0)
				close(sock);
		}
	public:
		int RecvLine(std::string &line)
		{
			char c = 'B';
			while(c != '\n')
			{
				ssize_t s = recv(sock,&c,1,0);
				if(s > 0)
				{
					if(c == '\r')
					{
						recv(sock,&c,1,MSG_PEEK);//窥探，不从队列中移除字符
						if(c == '\n')
						{
							recv(sock,&c,1,0);
						}
						else
						{
							c = '\n';
						}
					}
					line.push_back(c);
				}
				else
				{
					LOG(Warning,"recv request error!");
				}
			}
			return line.size();
		}
		void RecvHttpRequestLine(std::string &request_line)
		{
			RecvLine(request_line);
		}
		void RecvHttpRequestHeader(std::string &request_header)
		{
			std::string line = "";
			do
			{
				line = "";
				RecvLine(line);
				if(line != "\n")
				{
					request_header += line;
				}
			}while(line != "\n");
		}

		//读取http请求的请求行,请求报头,包括空行
		void RecvHttpRequest(HttpRequest *rq)
		{
			std::string request_line;
			std::string request_header;

			RecvHttpRequestLine(request_line);
			RecvHttpRequestHeader(request_header);

			rq->SetRequestLine(request_line);
			rq->SetRequestHeader(request_header);
		}
		//读取http请求的请求正文,正文放进
		void RecvHttpBody(HttpRequest *rq)
		{
			int content_length = rq->GetContentLength();
			if(content_length > 0)
			{
				std::string body;
				char c;
				while(content_length > 0)
				{
					recv(sock,&c,1,0);
					body.push_back(c);
					content_length--;
				}
				rq->SetRequestBody(body);
			}
			rq->SetUrlToPath();
			rq->SetCGI();
		}
		void SendResponse(HttpRequest *rq,HttpResponse *rsp)
		{
			std::string line = rsp->GetResponseLine();
			line += rsp->GetResponseHeader();
			line += rsp->GetBlank();
			send(sock,line.c_str(),line.size(),0);

			sendfile(sock,rq->GetFd(),nullptr,rq->GetFileSize());
			
		}
	private:
		int sock;
};



class Entry
{
	public:
		static void MakeResponse(HttpRequest *rq,HttpResponse *rsp)
		{
			if(rq->IsCGI())
			{

			}
			else
			{
				std::string line = "HTTP/1.0 200 OK\n";
				rsp->SetResponseLine(line);
				line = "Content-Type: ";
				line += Util::SuffixToType(rq->GetSuffix());
				line += "\r\n";
				rsp->AddResponseHeader(line);
				line = "Content-Length: ";
				line += Util::IntToString(rq->GetFileSize());
				line += "\r\n";
				rsp->AddResponseHeader(line);
				rq->OpenResources();
			}
		}
		static void ProcessNormal(Connect *conn,HttpRequest *rq,HttpResponse *rsp)
		{
			//没有cgi,没有query_string,不是post
			MakeResponse(rq,rsp);
			conn->SendResponse(rq,rsp);
		}
		static void* HandlerRequest(void *arg)
		{
			//int sock = (int)arg;
			int *p = (int*)arg;
			int sock = *p;
			delete p;

			int code = 200;	

			Connect *conn = new Connect(sock);
			HttpRequest *rq = new HttpRequest();
			HttpResponse *rsp = new HttpResponse();

			conn->RecvHttpRequest(rq);
			rq->RequestLineParse();

			rq->RequestHeaderParse();

			//Method is OK?GET  POST
			if(!rq->IsMethodOk())
			{
				code = 404;
				LOG(Error,"request method is not ok!");
				goto end;
				//TODO
			}
			//分析url:path,paramter
			//rq->RequestHeaderParse();
			//域名/文件资源?x=xx&&yyy=YY
			
			if(rq->IsPost())
			{
				//post
				conn->RecvHttpBody(rq);
				//request请求全部读完
			}
			if(rq->IsGet())
			{
				//分析url:path,paramter
				rq->UrlParse();
			}
			

			//分析请求资源是否合法
			//rq->path
			if(!rq->PathIsLegal())
			{
				code = 404;
				LOG(Warning,"Path is not Legal!");
				goto end;
				//TODO

			}
			if(rq->IsCGI())
			{
				LOG(Normal,"exec by cgi!");
			}
			else
			{
				LOG(Normal,"exec by non cgi");
				ProcessNormal(conn,rq,rsp);
			}
			//rq->Show();

			//recv request
			//parse request
			//make response
			//send response
		end:
			delete conn;
			delete rq;
			delete rsp;
			
			return nullptr;
		}
};
