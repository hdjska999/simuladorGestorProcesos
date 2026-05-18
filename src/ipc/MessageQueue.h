
#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <queue>
#include <string>

class MessageQueue {
private:
    std::queue<std::string> messages;

public:
    void send(std::string msg);
    void receive();
};


#endif