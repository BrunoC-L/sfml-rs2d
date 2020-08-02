#include "debug.h"
#include <iostream>

void print(const Vector3f v) {
	cout << v.x << ", " << v.y << ", " << v.z << std::endl;
}

void print(const Vector2f v) {
	cout << v.x << ", " << v.y << std::endl;
}

void print(const Vector3i v) {
	cout << v.x << ", " << v.y << ", " << v.z << std::endl;
}

void print(const Vector2i v) {
	cout << v.x << ", " << v.y << std::endl;
}

void print(const VPixel v) {
	cout << v.x << ", " << v.y << ", " << v.z << std::endl;
}

void print(const VTile v) {
	cout << v.x << ", " << v.y << ", " << v.z << std::endl;
}

void print(const VChunk v) {
	cout << v.x << ", " << v.y << ", " << v.z << std::endl;
}