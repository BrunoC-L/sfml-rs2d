#pragma once
#include "eventObserver.h"
#include <functional>
#include <vector>
using namespace std;

template <typename T>
class EventEmitter {
private:
	vector<EventObserver<T>*> subscribers;
public:
	void subscribe(EventObserver<T>* obv);
	void unsubscribe(EventObserver<T>* obv);
	void emit(T* t);
};
