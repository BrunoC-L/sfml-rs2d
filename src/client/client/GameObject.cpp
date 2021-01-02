#include "GameObject.h"

GameObject::GameObject(int id, function<void(VTile, int)> updateObjectTexture) : id(id), updateObjectTexture(updateObjectTexture) {
}
