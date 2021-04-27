#include "GameObject.h"

GameObject::GameObject(int id, std::function<void(VTile, int)> updateObjectTexture) : id(id), updateObjectTexture(updateObjectTexture) {
}
