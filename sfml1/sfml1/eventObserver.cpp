#include "eventObserver.h"

template <class T>
EventObserver<T>::EventObserver(function<void(T*)> f) {
	this->f = f;
}
