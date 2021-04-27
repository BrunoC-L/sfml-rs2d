#pragma once
#include <functional>

class OnExit {
	std::function<void()> f;
public:
	OnExit(std::function<void()> f) : f(f) {}
	~OnExit() {
		f();
	}
};
