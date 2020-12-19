#pragma once
#include "eventEmitter.h"

template <typename T>
void EventEmitter<T>::subscribe(EventObserver<T>* obv) {
	subscribers.push_back(obv);
}

template <typename T>
void EventEmitter<T>::unsubscribe(EventObserver<T>* obv) {
	for (int index = subscribers.size() - 1; index >= 0; --index)
		if (obv == subscribers[index])
			subscribers.erase(subscribers.begin() + index);
}

template <typename T>
void EventEmitter<T>::emit(T* t) {
	for (int index = subscribers.size() - 1; index >= 0; --index)
		subscribers[index]->f(t);
}
