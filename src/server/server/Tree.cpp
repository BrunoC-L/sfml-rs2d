#include "Tree.h"

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

int Tree::getState() {
    return UP;
}

std::string Tree::getName() {
    return "Tree";
}
