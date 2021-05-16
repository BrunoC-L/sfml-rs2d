#include "tree.h"
#include "tile.h"

void Tree::collect(const std::shared_ptr<User>& user) {
}

bool Tree::requirementsMet(const std::shared_ptr<User>& user) {
    return true;
}

void Tree::giveExperience(const std::shared_ptr<User>& user) {
}

void Tree::examineUp(const std::shared_ptr<User>& user) {
}

void Tree::examineStump(const std::shared_ptr<User>& user) {
}

Tree::Tree(JSON&& json, const std::shared_ptr<Tile>& tile) : AbstractTree(std::move(json), tile) {
    tile->position;
}

int Tree::getState() {
    return state;
}

std::string Tree::getName() {
    return "Tree";
}
