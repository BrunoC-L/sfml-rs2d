#pragma once
#include <functional>
#include <vector>

class Event {
public:
	virtual void emit() = 0;
};

template <class T>
class _EventObserver {
public:
	std::function<void(T&)> f;
	_EventObserver(std::function<void(T&)> f) {
		this->f = f;
	}
};

template <class T>
class EventObserver {
	void subscribe() {
		if (observer != nullptr)
			T::getEmitter().subscribe(observer);
	}
	void unsubscribe() {
		if (observer != nullptr)
			T::getEmitter().unsubscribe(observer);
	}
	std::shared_ptr<_EventObserver<T>> observer;
public:
	EventObserver(std::function<void(T&)> f) {
		observer = std::make_shared<_EventObserver<T>>(f);
		subscribe();
	}
	EventObserver() {

	}
	~EventObserver() {
		unsubscribe();
	}
	void set(std::function<void(T&)> f) {
		unsubscribe();
		observer = std::make_shared<_EventObserver<T>>(f);
		subscribe();
	}
};

template <typename T>
class EventEmitter {
private:
	std::vector<std::shared_ptr<_EventObserver<T>>> subscribers;
public:
	void subscribe(std::shared_ptr<_EventObserver<T>> obv) {
		subscribers.push_back(obv);
	}
	void unsubscribe(std::shared_ptr<_EventObserver<T>> obv) {
		for (int index = subscribers.size() - 1; index >= 0; --index)
			if (obv == subscribers[index])
				subscribers.erase(subscribers.begin() + index);
	}
	void emit(T& t) {
		// if someone unsuscribes during his callback, the next subscriber won't get the notification (or if a callback unsubscribes another!)
		for (int index = 0; index < subscribers.size(); ++index)
			subscribers[index]->f(t);
	}

	void clear() {
		subscribers = {};
	}
};

#define MEMBER_SET(x) this->x = x;
#define MEMBER_INITIALIZER(x) x(x)

#define EVENT_CLASS(_type, _parents, _members, _parameters, _members_set) \
class _type _parents {\
public:\
	_members\
	_type(_parameters) {\
	_members_set\
	}\
	static EventEmitter<_type>& getEmitter() {\
		static EventEmitter<_type> emitter;\
		return emitter;\
	}\
	static void subscribe(std::shared_ptr<_EventObserver<_type>> obv) {\
		getEmitter().subscribe(obv);\
	}\
	static void unsubscribe(std::shared_ptr<_EventObserver<_type>> obv) {\
		getEmitter().unsubscribe(obv);\
	}\
	void emit() {\
		getEmitter().emit(*this);\
	}\
	static void clear() {\
		getEmitter().clear();\
	}\
};\

