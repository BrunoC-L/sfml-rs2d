#pragma once

template <typename T, typename U, typename V>
bool clamp(T& unknown, U lower, V upper) {
	if (unknown < lower) {
		unknown = lower;
		return false;
	}
	if (unknown > upper) {
		unknown = upper;
		return false;
	}
	return true;
}
