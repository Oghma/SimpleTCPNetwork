#include "TCPServer.h"

std::mutex TCPServer::messageMutex;
std::condition_variable TCPServer::cv;

void TCPServer::task(int newSock) {
    int n;
    char msg[MAXPACKETSIZE];

    while(true) {
	n = recv(newSock, msg, MAXPACKETSIZE, 0);
	if(n == 0) {
	    close(newSock);
	    break;
	}
	msg[n] = 0;

	std::lock_guard<std::mutex> lk(messageMutex);
	mtx.lock();
	messages.push(Message(newSock, std::string(msg)));
	mtx.unlock();
	cv.notify_all();
    }
}

void TCPServer::setup(int port, int connections) {
    sock=socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(port);

    bind(sock,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(sock,connections);
}

void TCPServer::receive() {
    int newSock;
    struct sockaddr_in clientAddress;
    socklen_t clientSize = sizeof(clientAddress);

    while (true) {
	newSock = accept(sock, (struct sockaddr *)&clientAddress, &clientSize);
	std::thread th(&TCPServer::task, this, newSock);
	th.detach();
    }
}

Message TCPServer::getMessage() {
    mtx.lock();
    Message msg = messages.front();
    messages.pop();
    mtx.unlock();
    return msg;
}

void TCPServer::sendMessage(Message message) {
    std::string msg = message.getMessage();
    send(message.getSocket(), msg.c_str(), msg.length(),  0);
}

void TCPServer::sendMessage(int sock, std::string message) {
    send(sock, message.c_str(), message.length(), 0);
}

void TCPServer::detach() {
    close(sock);
}

bool TCPServer::message() {
    mtx.lock();
    bool msg = messages.empty();
    mtx.unlock();
    return !msg;
}
