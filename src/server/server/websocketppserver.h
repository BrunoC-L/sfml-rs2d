/*
 * Copyright (c) 2015, Peter Thorson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the WebSocket++ Project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <functional>

typedef websocketpp::server<websocketpp::config::asio> server;

class WebSocketServer {
protected:
    unsigned port;
    server m_endpoint;
public:
    WebSocketServer(unsigned port) : port(port) {
        //m_endpoint.set_error_channels(websocketpp::log::elevel::all);
        m_endpoint.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);

        m_endpoint.init_asio();

        m_endpoint.set_message_handler(std::bind(
            &WebSocketServer::handler, this,
            std::placeholders::_1, std::placeholders::_2
        ));
    }

    virtual void handler(websocketpp::connection_hdl hdl, server::message_ptr msg) = 0;

    virtual void run() {
        m_endpoint.listen(port);
        m_endpoint.start_accept();
        m_endpoint.run();
    }
};
//
//int main() {
//    WebSocketServer s(9002);
//    s.run();
//    return 0;
//}

#include "json.h"
#include <SFML/Network.hpp>

class WebSocket : public Socket {
    server* m_endpoint;
    websocketpp::connection_hdl hdl;
public:
    WebSocket(server* m_endpoint, websocketpp::connection_hdl hdl) : m_endpoint(m_endpoint), hdl(hdl) {}

    virtual void send(const std::string& msg) {
        m_endpoint->send(hdl, msg, websocketpp::frame::opcode::TEXT);
    }

    virtual void disconnect() override {
        m_endpoint->close(hdl, 0, ":(");
    }

    virtual sf::TcpSocket& getSFMLIMPL() {
        throw std::runtime_error("WebSocket cant getSFMLIMPL");
    }
};

class JSONWebSocketServer : public WebSocketServer {
protected:
    std::function<void(std::exception&, std::shared_ptr<Socket>)> onError;
    std::unordered_map<std::string, std::vector<std::function<void(std::shared_ptr<Socket>, JSON&)>>> callbacks;
    std::function<void(std::shared_ptr<Socket>)> onConnect;
    std::function<void(std::shared_ptr<Socket>)> onDisconnect;
    std::vector<std::shared_ptr<websocketpp::connection<websocketpp::config::asio>>> connections;
    std::vector<std::shared_ptr<Socket>> sockets;
    std::thread runner;
public:
    JSONWebSocketServer(
        unsigned port,
        std::function<void(std::exception&, std::shared_ptr<Socket>)> onError,
        std::function<void(std::shared_ptr<Socket>)> onConnect,
        std::function<void(std::shared_ptr<Socket>)> onDisconnect
    ) :
        WebSocketServer(port), onConnect(onConnect), onDisconnect(onDisconnect), onError(onError)
    {
    }

    void start() {
        runner = std::thread([&]() { run(); });
    }

    virtual void handler(websocketpp::connection_hdl hdl, server::message_ptr msg) override {
        auto con = m_endpoint.get_con_from_hdl(hdl);
        auto it = std::find(connections.begin(), connections.end(), con); // todo binary search
        unsigned index = connections.size();
        if (it == connections.end()) {
            connections.push_back(con); // todo sorted insert
            auto ws = std::make_shared<WebSocket>(&m_endpoint, hdl);
            sockets.push_back(ws);
            onConnect(ws);
        } else
            index = it - connections.begin();
        try {
            std::string payload = msg->get_payload();
            //std::cout << payload << "\n";
            JSON json(msg->get_payload());
            std::string type = json["type"].asString();
            //std::cout << type << "\n";
            JSON data = json["data"];
            receive(type, sockets.at(index), data);
        }
        catch (std::exception& e) {
            onError(e, sockets.at(index));
        }
    }

    void on(std::string msgType, std::function<void(std::shared_ptr<Socket>, JSON&)> callback) {
        callbacks[msgType].push_back(callback);
    }

    void receive(std::string msgType, std::shared_ptr<Socket> socket, JSON json) {
        for (auto callback : callbacks[msgType])
            callback(socket, json);
    }
};
