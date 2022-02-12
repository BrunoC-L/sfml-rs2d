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
    walkObserver.set([&](const WalkClickEvent::Data& ev) {
        walk(ev.pos);
    });

    loginObserver.set([&](const LoginEvent::Data& ev) {
        auto& data = ev.json;
        auto id = data.get("id");
        setID(id.asInt());
        JSON x_ = data.get("position").get("x"), y_ = data.get("position").get("y");
        int x = x_.asInt(), y = y_.asInt();
        player->setIntPosition(VTile(x, y));
        player->setPosition(VTile(x, y));
    });

    tabObserver.set([&](const TabKeyPressedEvent& ev) {
        loginData.typingUsername = !loginData.typingUsername;
    });

    enterObserver.set([&](const EnterKeyPressedEvent::Data& ev) {
        if (!loginData.typingUsername)
            login();
        loginData.typingUsername = !loginData.typingUsername;
    });

    backspaceObserver.set([&](const BackspaceKeyPressedEvent::Data& ev) {
        if (loginData.typingUsername)
            loginData.username = loginData.username.substr(0, loginData.username.length() - 1);
        else
            loginData.password = loginData.password.substr(0, loginData.password.length() - 1);
    });

    letterObserver.set([&](const LetterKeyPressedEvent::Data& ev) {
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
    JSON data;
    auto username = player->getUserNamePw().first;
    data["username"] = "'" + username + "'";
    socket->send("salts request", data);
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
