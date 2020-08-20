#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include <iostream>
using namespace sf;
using namespace std;

void print(const Vector3f v);

void print(const Vector2f v);

void print(const Vector3i v);

void print(const Vector2i v);

void print(const VPixel v);

void print(const VTile v);

void print(const VChunk v);


template <typename T>
void print(T t) {
	cout << t << '\n';
}

template <typename T>
void print() {
}

template <typename T, typename... Ts>
void print(T t, Ts... ts) {
	print(t);
	print(ts...);
}
