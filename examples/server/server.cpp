#include <iostream>
#include <string>
#include "TCPServer.h"

using namespace std;

TCPServer tcp;

void loop() {
    while(true) {
        std::unique_lock<std::mutex> lk(TCPServer::messageMutex);
        TCPServer::cv.wait(lk, []{return tcp.message();});
        lk.unlock();

        Message msg = tcp.getMessage();
        string str = msg.getMessage();
        cout << "Message:" << str << endl;
        tcp.sendMessage(msg.getSocket(), " [client message: "+str+"]");
    }
    tcp.detach();
}

int main() {
    tcp.setup(11999,10);

    std::thread th(loop);
    th.detach();

    tcp.receive();
    return 0;
}
