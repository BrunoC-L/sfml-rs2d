#pragma once

class GameTick {
private:
	unsigned _tick = 0;
public:
	static void tick() {
		static GameTick instance;
		instance._tick += 1;
	}
	static unsigned get() {
		static GameTick instance;
		return instance._tick;
	}
};
