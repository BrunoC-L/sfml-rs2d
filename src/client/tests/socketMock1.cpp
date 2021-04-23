#include "pch.h"
#include "socketMock1.h"
#include "login.h"

SocketMock1::SocketMock1(AbstractServiceProvider* provider) : Socket(provider, "1", 1) {
    provider->set("Socket", this);
}

void SocketMock1::init() {
    on("login",
        [&](JSON& data) {
            LoginEvent(data).emit();
        }
    );
}

void SocketMock1::connect(std::string ip, unsigned port) {

}

void SocketMock1::disconnect() {

}

void SocketMock1::mockReceiveFromServer(JSON json) {
    receive(json["type"].asString(), json["data"]);
}
