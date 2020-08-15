#include "tree.h"

Tree::Tree(int id, VTile position, function<void(VTile, int)> updateObjectTexture) : GameRessourceObject(id, position, 2, updateObjectTexture, "1;logs;1;false;1") {

}
