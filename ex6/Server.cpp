/*
 * Server.cpp
 *
 * Author: 337904619 Margarita Kilinkarov 201283033 Gitit Shapira
 */
#include "Server.h"

Server::Server(int port)throw (const char*) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        throw "Socket failure!";
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if (bind(fd, (struct sockaddr *) &server, sizeof(server)) < 0)
        throw "Bind failure!";
    if (listen(fd, clientLimit) < 0)
        throw "Listen failure!";

}

void Server::start(ClientHandler& ch)throw(const char*){
    t = new thread([&ch,this](){// lambda expression, using this for _fd and _client
        while(!stop_s){
            socklen_t clientSize= sizeof (client);
            int aClient = accept(fd, (struct  sockaddr*)&client,&clientSize);
            if(aClient < 0)
                throw "Accept failure!";
            ch.handle(aClient);
            close(aClient);
        }
        close(fd);
    });
}

void Server::stop(){
    stop_s = true;
	t->join(); // do not delete this!
}

Server::~Server() {
}

