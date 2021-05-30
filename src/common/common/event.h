#pragma once
#include <functional>
#include <vector>
#include <memory>

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
		T::getEmitter().subscribe(observer);
		_isSet = true;
	}
	void unsubscribe() {
		T::getEmitter().unsubscribe(observer);
		_isSet = false;
		observer.reset();
	}
	bool _isSet = false;
	std::shared_ptr<_EventObserver<T>> observer;
public:
	~EventObserver() {
		if (observer)
			unsubscribe();
	}
	void set(std::function<void(T&)> f) {
		observer = std::make_shared<_EventObserver<T>>(f);
		subscribe();
	}
	void unset() {
		unsubscribe();
	}
	bool isSet() {
		return _isSet;
	}
};

template <typename T>
class EventEmitter {
private:
	std::vector<std::shared_ptr<_EventObserver<T>>>   subscribers;
	std::vector<std::shared_ptr<_EventObserver<T>>> unsubscribers;
public:
	void subscribe(std::shared_ptr<_EventObserver<T>> obv) {
		subscribers.push_back(obv);
	}
	void unsubscribe(std::shared_ptr<_EventObserver<T>> obv) {
		unsubscribers.push_back(obv);
	}
	void emit(T& t) {
		// if someone unsuscribes during his callback, the next subscriber won't get the notification (or if a callback unsubscribes another!)
		for (const auto& obv : subscribers)
			if (std::find(unsubscribers.begin(), unsubscribers.end(), obv) == unsubscribers.end())
				obv->f(t);
		for (const auto& obv : unsubscribers)
			subscribers.erase(std::find(subscribers.begin(), subscribers.end(), obv));
		unsubscribers = {};
	}

	void clear() {
		subscribers = {};
	}
};

#define MEMBER_SET(x) this->x = x;
#define MEMBER_INITIALIZER(x) x(x)

#define EVENT_CLASS(_type, _parents, _members, _parameters, _members_set, _members_initializers) \
class _type _parents {\
public:\
	_members\
	_type(_parameters) _members_initializers {\
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

