#include "httpServer.hpp"

int main(int argc, char *argv[])
{
    if(argc != 2){
        std::cout << "Usage: "<< argv[0] <<" port" << std::endl;
        return 1;
    }

    httpServer *sp = new httpServer(atoi(argv[1]));

    sp->initServer();
    sp->Start();

    delete sp;
    return 0;
}
