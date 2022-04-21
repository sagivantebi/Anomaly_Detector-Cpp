/* Authors:
 * Ben Ganon 
 * Sagiv Antebi 
 */

#include "Server.h"

string socketIO::read() {
    char cBuffer = 0;
    string sToReturn = "";
    while (cBuffer != '\n') {
        recv(clientID, &cBuffer, sizeof(char), 0);
        sToReturn += cBuffer;
    }
    return sToReturn;
}

void socketIO::write(string text) {
    const char *stringToWrite = text.c_str();
    send(clientID, stringToWrite, strlen(stringToWrite), 0);
}

void socketIO::write(float num) {
    ostringstream stringStream;
    stringStream << num;
    string s(stringStream.str());
    write(s);
}

void socketIO::read(float *num) {
    recv(clientID, num, sizeof(float), 0);
}

void sigHandle(int sigNum) {
    return;
}

/**
 * Server - initial the server
 * @param port - num of port
 */
Server::Server(int port) throw(const char *) {
    this->stopped = false;
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->fd < 0)
        throw;
    this->server.sin_family = AF_INET;
    this->server.sin_addr.s_addr = INADDR_ANY;
    this->server.sin_port = htons(port);
    if (bind(this->fd, (struct sockaddr *) &this->server, sizeof(this->server)) < 0)
        throw;
    if (listen(fd, 3) < 0)
        throw;
}

/**
 * start - start method will start the connection with the given client
 * @param ch - the client to connect
 */
void Server::start(ClientHandler &ch) throw(const char *) {
    this->t = new thread([&ch, this]() {
        signal(SIGALRM, sigHandle);
        while (!this->stopped) {
            alarm(1);
            socklen_t clientSize = sizeof(this->client);
            int clientOne = accept(fd, (struct sockaddr *) &this->client, &clientSize);
            if (clientOne < 0)
                throw;
            ch.handle(clientOne);
            close(clientOne);
            alarm(0);
        }
        close(this->fd);
    });
}

void Server::stop() {
    this->stopped = true;
    this->t->join();
}

Server::~Server() {
}

