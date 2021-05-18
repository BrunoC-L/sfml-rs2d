#include "tree.h"
#include "tile.h"
#include "goToObjectRequestEvent.h"

void Tree::collect(const std::shared_ptr<User>& user) {
    GoToObjectRequest(user, this, [&]() {
        std::cout << "Hello World!\n";
    }).emit();
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

Tree::Tree(JSON&& json, Tile* tile) : AbstractTree(std::move(json), tile) {
}

int Tree::getState() {
    return state;
}

std::string Tree::getName() {
    return "Tree";
}
