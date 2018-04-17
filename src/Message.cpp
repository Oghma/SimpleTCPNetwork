#include "Message.h"

Message::Message(int sock, std::string message) {
  this->sock = sock;
  this->message = message;
}

std::string Message::getMessage() {
  return message;
}

int Message::getSocket() {
  return sock;
}
