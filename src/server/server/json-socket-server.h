#pragma once
#include "socket-server.h"
#include "json.h"
#include "print.h"

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
				{
					std::ostringstream ss;
					ss << "Socket Message Queue Thread: " << std::this_thread::get_id() << std::endl;
					print(ss);
				}
				while (!stopped) {
					std::unique_lock<std::mutex> lock(waiter);
					cv.wait(lock, [&]() { return messageQueue.size() != 0 || stopped; });
					if (stopped)
						break;
					queueMutex.lock();
					auto msg = messageQueue[0];
					messageQueue.erase(messageQueue.begin());
					queueMutex.unlock();
					receive(msg);
				}
				{
					std::ostringstream ss;
					ss << "Socket Message Queue Thread " << std::this_thread::get_id() << " Exiting" << std::endl;
					print(ss);
				}
			}
		);
	}

	void stop() {
		server.stop();
		stopped = true;
		cv.notify_one();
		logicThread.join();
	}
};
