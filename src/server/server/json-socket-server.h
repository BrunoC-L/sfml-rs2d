#pragma once
#include "socket-server.h"
#include "json.h"

struct QueueMessage {
	sf::TcpSocket* socket;
	std::string message;
};

class JsonSocketServer {
public:
	SocketServer server;
	std::unordered_map<std::string, std::vector<std::function<void(sf::TcpSocket*, JSON)>>> callbacks;
	std::vector<QueueMessage> messageQueue;
	sf::Mutex queueMutex;
	std::thread logicThread;
	bool stopped = false;
	std::function<void(std::exception, QueueMessage)> onError;

	JsonSocketServer(unsigned port, std::string endMessage, std::function<void(std::exception, QueueMessage)> onError, std::function<void(sf::TcpSocket*)> onDisconnect) :
		server(port, endMessage, [&](sf::TcpSocket* socket, std::string msg) { queue(socket, msg); }, onDisconnect)
	{
		this->onError = onError;
	}

	void on(std::string msgType, std::function<void(sf::TcpSocket*, JSON)> callback) {
		callbacks[msgType].push_back(callback);
	}

	void queue(sf::TcpSocket* socket, std::string msg) {
		queueMutex.lock();
		messageQueue.push_back({ socket, msg });
		queueMutex.unlock();
	}

	void receive(QueueMessage qm) {
		try {
			JSON json(qm.message);
			std::string type = json["type"].asString();
			JSON data = json["data"];
			receive(type, qm.socket, data);
		}
		catch (std::exception e) {
			onError(e, qm);
		}
	}

	void receive(std::string msgType, sf::TcpSocket* socket, JSON json) {
		for (auto callback : callbacks[msgType])
			callback(socket, json);
	}

	void start() {
		stopped = false;
		server.start();
		logicThread = std::thread(
			[&]() {
				while (!stopped) {
					if (messageQueue.size() == 0)
						continue;
					queueMutex.lock();
					auto msg = messageQueue[0];
					messageQueue.erase(messageQueue.begin());
					queueMutex.unlock();
					receive(msg);
				}
			}
		);
	}

	void stop() {
		stopped = true;
		server.stop();
		logicThread.join();
	}
};
