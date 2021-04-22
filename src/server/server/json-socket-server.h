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
	std::unordered_map<std::string, std::vector<std::function<void(sf::TcpSocket*, JSON&)>>> callbacks;
	std::vector<QueueMessage> messageQueue;
	std::mutex waiter;
	std::condition_variable cv;
	std::mutex queueMutex;
	std::thread logicThread;
	bool stopped = false;
	std::function<void(std::exception&, QueueMessage)> onError;

	JsonSocketServer(
		unsigned port,
		std::function<void(std::exception&, QueueMessage)> onError,
		std::function<void(sf::TcpSocket*)> onConnect,
		std::function<void(sf::TcpSocket*)> onDisconnect
	) :
		server(port, [&](sf::TcpSocket* socket, std::string msg) { queue(socket, msg); }, onConnect, onDisconnect), onError(onError)
	{ }

	void on(std::string msgType, std::function<void(sf::TcpSocket*, JSON&)> callback) {
		callbacks[msgType].push_back(callback);
	}

	void queue(sf::TcpSocket* socket, std::string msg) {
		queueMutex.lock();
		messageQueue.push_back({ socket, msg });
		queueMutex.unlock();
		cv.notify_one();
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
					std::unique_lock<std::mutex> lock(waiter);
					cv.wait(lock, [&]() { return messageQueue.size() != 0; });
					lock.unlock();
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
