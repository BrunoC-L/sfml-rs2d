#pragma once
#include "socket-server.h"
#include "json.h"
#include "print.h"
#include "socket.h"

struct QueueMessage {
	std::shared_ptr<Socket> socket;
	std::string message;
};

class JsonSocketServer {
public:
	SocketServer server;
	std::unordered_map<std::string, std::vector<std::function<void(std::shared_ptr<Socket>, JSON&)>>> callbacks;
	std::vector<QueueMessage> messageQueue;
	std::mutex waiter;
	std::condition_variable cv;
	std::mutex queueMutex;
	std::thread logicThread;
	bool stopped = false;
	std::function<void(std::exception&, std::shared_ptr<Socket>)> onError;

	JsonSocketServer(
		unsigned port,
		std::function<void(std::exception&, std::shared_ptr<Socket>)> onError,
		std::function<void(std::shared_ptr<Socket>)> onConnect,
		std::function<void(std::shared_ptr<Socket>)> onDisconnect
	) :
		server(port, [&](std::shared_ptr<Socket> socket, std::string msg) { queue(socket, msg); }, onConnect, onDisconnect), onError(onError)
	{ }

	void on(std::string msgType, std::function<void(std::shared_ptr<Socket>, JSON&)> callback) {
		callbacks[msgType].push_back(callback);
	}

	void queue(std::shared_ptr<Socket> socket, std::string msg) {
		{
			std::lock_guard<std::mutex> lock(queueMutex);
			messageQueue.push_back({ socket, msg });
		}
		cv.notify_one();
	}

	void receive(QueueMessage qm) {
		try {
			JSON json(qm.message);
			std::string type = json["type"].asString();
			JSON data = json["data"];
			receive(type, qm.socket, data);
		}
		catch (std::exception& e) {
			onError(e, qm.socket);
		}
	}

	void receive(std::string msgType, std::shared_ptr<Socket> socket, JSON json) {
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
					QueueMessage msg;
					{
						std::lock_guard<std::mutex> lock(queueMutex);
						msg = messageQueue[0];
						messageQueue.erase(messageQueue.begin());
					}
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
