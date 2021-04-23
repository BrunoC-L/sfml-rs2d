#pragma once
#include <functional>
#include "abstractService.h"

class AbstractPlayer : public AbstractService {
	virtual std::pair<std::string, std::string> getCredentials() const = 0;
public:
	virtual void setID(int id) = 0;
	virtual const int& getID() = 0;
	virtual void setPosition(VTile position) = 0;
	virtual void setIntPosition(VTile position) = 0;
	virtual const VTile& getPosition() = 0;
	virtual const VTile& getIntPosition() = 0;
	virtual std::pair<std::string, std::string> getUserNamePw() const = 0;
	virtual void setSalts(std::string tempsalt, std::string permsalt) = 0;
	virtual void login() = 0;
	virtual void signUp() = 0;
};
