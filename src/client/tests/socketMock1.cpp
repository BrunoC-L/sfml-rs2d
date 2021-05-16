#include "pch.h"
#include "socketMock1.h"
#include "login.h"

SocketMock1::SocketMock1(ServiceProvider* provider) : Socket(provider, "0", 0) {
    provider->set(SOCKET, this);
}

void SocketMock1::init() {
    on("login",
        [&](std::shared_ptr<const JSON> data) {
            LoginEvent(*data).emit();
        }
    );
}

void SocketMock1::connect(std::string ip, unsigned port) {

}

void SocketMock1::disconnect() {

}

void SocketMock1::mockReceiveFromServer(JSON& json) {
    JSON* dummy = new JSON();
    std::swap(*dummy, json["data"]);
    receive(json["type"].asString(), std::shared_ptr<const JSON>(dummy));
}
