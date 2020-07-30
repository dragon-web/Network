#pragma once
#include <pthread.h>
#include "comm.hpp"

using namespace std;

class httpServer{
    private:
        int listen_sock;
        int port;
    public:
        httpServer(int _port):port(_port){}

        void initServer()
        {
            listen_sock = Sock::Socket();
            Sock::Bind(listen_sock, port);
            Sock::Listen(listen_sock, 5);
        }
        static void *handlerRequest(void *arg)
        {
            pthread_detach(pthread_self());
            int *p = (int*)arg;
            int sock = *p;
            delete p;
            cout << "sock: "<< sock << endl;
            char request[10240];
            memset(request, 0, sizeof(request));

            ssize_t s = read(sock, request, sizeof(request));
            if(s > 0){
                cout <<"###################################request begin##############################" << endl;
                cout << request << endl;
                cout <<"################################### request end ##############################" << endl;
            }
            //response TODO
            //string text = "<form action=\"\\\" method=\"GET\">First name:<br><input type=\"text\" name=\"firstname\"><br>Passwd:<br><input type=\"password\" name=\"passwd\"><br><input type=\"submit\" value=\"Login\"></form>";
            string text = "<html><h1>hello http Liyanran!!!</h1></html>";
            string status_line = "HTTP/1.1 200 OK\r\n";
            string resp_header = "Content-Type: text/html\r\n";
           // resp_header += "Location: https://www.w3school.com.cn/html/html_forms.asp/xxx\r\n";
            resp_header +="Content-Length: ";
            resp_header += to_string(text.size());
            resp_header += "\r\n";

            //resp_header += "Set-Cookie: sessionid=123456;path=/\r\n";
            //resp_header += "\r\n";

            resp_header += "\r\n";

            string response = status_line;
            response += resp_header;
            response += text;

            cout <<"###################################response begin##############################" << endl;
            cout << response << endl;
            cout <<"###################################response end ##############################" << endl;

            write(sock, response.c_str(), response.size());

            close(sock);
        }
        void Start()
        {
            for(;;){
                int sock = Sock::Accept(listen_sock);
                if(sock > 0){
                    cout << "get a new link..." <<endl;
                    int *p = new int(sock);
                    pthread_t tid;
                    pthread_create(&tid, nullptr, handlerRequest, (void*)p);
                }
            }
        }
        ~httpServer()
        {
            if(listen_sock >= 0){
                close(listen_sock);
            }
        }
};






