#include "pch.h"
#include "socketMock1.h"

SocketMock1::SocketMock1(AbstractServiceProvider* provider) : Socket(provider) {
    provider->set("Socket", this);
}

void SocketMock1::init() {

}

void SocketMock1::connect(std::string ip, unsigned port) {

}

void SocketMock1::disconnect() {

}

void SocketMock1::mockReceiveFromServer(JSON json) {
    receive(json["type"].asString(), json["data"]);
}
