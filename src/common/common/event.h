#pragma once
#include <functional>
#include <vector>
using namespace std;

class Event {
public:
	virtual void emit() = 0;
};

template <class T>
class EventObserver {
public:
	function<void(T*)> f;
	EventObserver(function<void(T*)> f) {
		this->f = f;
	}
};

template <typename T>
class EventEmitter {
private:
	vector<EventObserver<T>*> subscribers;
public:
	void subscribe(EventObserver<T>* obv) {
		subscribers.push_back(obv);
	}
	void unsubscribe(EventObserver<T>* obv) {
		for (int index = subscribers.size() - 1; index >= 0; --index)
			if (obv == subscribers[index])
				subscribers.erase(subscribers.begin() + index);
	}
	void emit(T* t) {
		for (int index = subscribers.size() - 1; index >= 0; --index)
			subscribers[index]->f(t);
	}
};

#define MEMBER_SET(x) this->x = x;
#define MEMBER_INITIALIZER(x) x(x)

#define EVENT_CLASS(_type, _parents, _members, _parameters, _members_set) \
class _type _parents {\
private:\
	static EventEmitter<_type>& getEmitter() {\
		static EventEmitter<_type> emitter;\
		return emitter;\
	}\
public:\
	_members\
	_type(_parameters) {\
	_members_set\
	}\
	static void subscribe(EventObserver<_type>* obv) {\
		getEmitter().subscribe(obv);\
	}\
	static void unsubscribe(EventObserver<_type>* obv) {\
		getEmitter().unsubscribe(obv);\
	}\
	void emit() {\
		getEmitter().emit(this);\
	}\
};\
