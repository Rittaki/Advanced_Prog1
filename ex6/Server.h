/*
 * Server.h
 *
 * Author: 337904619 Margarita Kilinkarov 201283033 Gitit Shapira
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>

#include <pthread.h>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"
#include "CLI.h"


using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        SocketIO socketIO(clientID);
        CLI cli(&socketIO);
        cli.start();

    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
    int clientLimit = 3;
    int fd;
    sockaddr_in server;
    sockaddr_in client;
    bool stop_s = false;

	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
