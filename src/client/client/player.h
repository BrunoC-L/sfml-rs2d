#pragma once
#include "units.h"
#include "teleportEvent.h"
#include "interactionClickEvent.h"
#include "service.h"
#include "login.h"
#include "keyPressedEvent.h"

struct LoginData {
	std::string username;
	std::string password;
	bool typingUsername = true;
};

class Player : public AbstractPlayer, public Service {
protected:
	int id = 0;
	VTile position;
	VTile intPosition;
    void walk(VTile pos);
	LoginData loginData;
	virtual std::pair<std::string, std::string> getCredentials(std::string tempsalt, std::string permsalt) const override;
	EventObserver<WalkClickEvent> walkObserver;
	EventObserver<LoginEvent> loginObserver;
	EventObserver<LetterKeyPressedEvent> letterObserver;
	EventObserver<BackspaceKeyPressedEvent> backspaceObserver;
	EventObserver<TabKeyPressedEvent> tabObserver;
	EventObserver<EnterKeyPressedEvent> enterObserver;
public:
    Player(ServiceProvider* provider);
    void init();
	virtual void setID(int id) ;
	virtual const int& getID();
	virtual void setPosition(VTile position);
	virtual void setIntPosition(VTile position);
	virtual const VTile& getPosition();
	virtual const VTile& getIntPosition();
	virtual std::pair<std::string, std::string> getUserNamePw() const override;
	virtual void login(std::string tempsalt, std::string permsalt) override;
	virtual void login() override;
	virtual void signUp() override;
};
