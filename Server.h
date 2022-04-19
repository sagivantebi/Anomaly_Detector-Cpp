/* Authors:
 * Ben Ganon - 318731007
 * Sagiv Antebi - 318159282
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<signal.h>
#include <sstream>
#include "CLI.h"

using namespace std;

class ClientHandler {
public:
    virtual void handle(int clientID) = 0;
};


class socketIO : public DefaultIO {
    int clientID;
public:

    socketIO(int clientID) : clientID(clientID) {}

    virtual string read();

    virtual void write(string text);

    virtual void write(float f);

    virtual void read(float *f);

};


class AnomalyDetectionHandler : public ClientHandler {
public:
    virtual void handle(int clientID) {
        socketIO sio(clientID);
        CLI cli(&sio);
        cli.start();
    }
};


class Server {
    int fd;
    sockaddr_in server;
    sockaddr_in client;
    thread *t;
    volatile bool stopped;

public:
    Server(int port) throw(const char *);

    virtual ~Server();

    void start(ClientHandler &ch) throw(const char *);

    void stop();
};

#endif
