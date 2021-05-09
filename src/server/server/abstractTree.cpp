#include "abstractTree.h"

VTile AbstractTree::size() {
	return VTile(2, 2);
}

std::vector<std::string> AbstractTree::getInteractions() {
	return { "Chop", "Examine" };
}

void AbstractTree::interact(const std::shared_ptr<User>& user, int objectState, int index) {
	if (objectState == UP && index == CHOP)
		collect(user);
	if (objectState == UP && index == EXAMINE)
		examineUp(user);
	if (objectState == STUMP && index == EXAMINE)
		examineStump(user);
}

int AbstractTree::getState() {
	return state;
}
