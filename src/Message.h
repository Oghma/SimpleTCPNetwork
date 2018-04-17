#ifndef SERVER_MESSAGE_H
#define SERVER_MESSAGE_H

#include <string>

class Message {
private:
    int sock;
    std::string message;

public:
    Message(int sock, std::string message);
    std::string getMessage();
    int getSocket();
};

#endif
