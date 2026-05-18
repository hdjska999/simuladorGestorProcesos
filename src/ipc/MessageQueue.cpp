
#include "MessageQueue.h"
#include <iostream>

using namespace std;

void MessageQueue::send(string msg) {
    messages.push(msg);
}

void MessageQueue::receive() {

    if (messages.empty()) {
        cout << "No hay mensajes" << endl;
        return;
    }

    cout << "Mensaje: " << messages.front() << endl;

    messages.pop();
}