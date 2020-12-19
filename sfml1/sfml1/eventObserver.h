#pragma once
#include <functional>
using namespace std;

template <class T>
class EventObserver {
private:
public:
	function<void(T*)> f;
	EventObserver(function<void(T*)> f);
};
