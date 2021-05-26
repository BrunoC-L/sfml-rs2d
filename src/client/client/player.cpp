#include "player.h"
#include "sha256.h"

Player::Player(ServiceProvider* provider): Service(provider) {
    provider->set(PLAYER, this);
    position = VTile();
}

void Player::init() {
    acquire();
    position = VTile();
    intPosition = VTile();
    walkObserver.set([&](WalkClickEvent& ev) {
        walk(ev.pos);
    });

    loginObserver.set([&](LoginEvent& ev) {
        auto& data = ev.json;
        auto id = data["id"];
        setID(id.asInt());
        JSON x = data["position"]["x"], y = data["position"]["y"];
        player->setIntPosition(VTile(x.asInt(), y.asInt()));
        player->setPosition(VTile(data["position"]["x"].asInt(), data["position"]["y"].asInt()));
    });

    tabObserver.set([&](TabKeyPressedEvent& ev) {
        loginData.typingUsername = !loginData.typingUsername;
    });



    enterObserver.set([&](EnterKeyPressedEvent& ev) {
        if (loginData.typingUsername)
            login();
        else
            loginData.typingUsername = true;
    });

    backspaceObserver.set([&](BackspaceKeyPressedEvent& ev) {
        if (loginData.typingUsername)
            loginData.username = loginData.username.substr(0, loginData.username.length() - 1);
        else
            loginData.password = loginData.password.substr(0, loginData.password.length() - 1);
    });

    letterObserver.set([&](LetterKeyPressedEvent& ev) {
        if (loginData.typingUsername)
            loginData.username += ev.letter;
        else
            loginData.password += ev.letter;
    });
}

void Player::setID(int id) {
	this->id = id;
}

const int& Player::getID() {
	return id;
}

void Player::setPosition(VTile position) {
    this->position = position;
}

void Player::setIntPosition(VTile position) {
    this->intPosition = position;
}

const VTile& Player::getPosition() {
    return position;
}

const VTile& Player::getIntPosition() {
    return intPosition;
}

std::pair<std::string, std::string> Player::getCredentials(std::string tempsalt, std::string permsalt) const {
    return { loginData.username, picosha2::hash256_hex_string(tempsalt + picosha2::hash256_hex_string(permsalt + picosha2::hash256_hex_string(loginData.password))) };
}

std::pair<std::string, std::string> Player::getUserNamePw() const {
    std::string hiddenPassword = "";
    auto len = loginData.password.length();
    hiddenPassword.reserve(len);
    for (int i = 0; i < len; ++i)
        hiddenPassword += "*";
    return {loginData.username, hiddenPassword};
}

void Player::login() {
    JSON json;
    json["type"] = "'salts request'";
    json["data"] = JSON();
    auto username = player->getUserNamePw().first;
    json["data"]["username"] = "'" + username + "'";
    socket->send(json);
}

void Player::login(std::string tempsalt, std::string permsalt) {
    JSON json;
    auto credentials = getCredentials(tempsalt, permsalt);
    json["username"] = credentials.first;
    json["passwordHash"] = credentials.second;
    socket->send("login", json);
    loginData.password = "";
    loginData.username = "";
}

void Player::signUp() {
    JSON json;
    json["username"] = "'" + loginData.username + "'";
    json["passwordHash"] = "'" + picosha2::hash256_hex_string(loginData.password) +"'";
    socket->send("sign up", json);
    loginData.password = "";
    loginData.username = "";
}

void Player::walk(VTile pos) {
    JSON json;
    json["x"] = std::to_string(pos.x);
    json["y"] = std::to_string(pos.y);
    json["id"] = std::to_string(id);
    socket->send("walk", json);
}
