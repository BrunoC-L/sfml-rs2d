#include "player.h"
#include "login.h"
#include "keyPressedEvent.h"
#include "sha256.h"

Player::Player(AbstractServiceProvider* provider): Service(provider) {
    provider->set("Player", this);
    position = VTile();
}

void Player::init() {
    acquire();
    VTile lumbridge(18 * AbstractMeasures::TilesPerChunk + 20, 13 * AbstractMeasures::TilesPerChunk + 37, 0);
    position = lumbridge;
    intPosition = lumbridge;
    WalkClickEvent::subscribe(new EventObserver<WalkClickEvent>(
        [&](WalkClickEvent* ev) {
            walk(ev->pos);
        }
    ));

	LoginEvent::subscribe(
		new EventObserver<LoginEvent>(
			[&](LoginEvent* ev) {
				auto& data = ev->json;
				int id = data.asInt();
				setID(id);
			}
		)
	);

    TabKeyPressedEvent::subscribe(
        new EventObserver<TabKeyPressedEvent>(
            [&](TabKeyPressedEvent* ev) {
                loginData.typingUsername = !loginData.typingUsername;
            }
        )
    );

    BackspaceKeyPressedEvent::subscribe(
        new EventObserver<BackspaceKeyPressedEvent>(
            [&](BackspaceKeyPressedEvent* ev) {
                if (loginData.typingUsername)
                    loginData.username = loginData.username.substr(0, loginData.username.length() - 1);
                else
                    loginData.password = loginData.password.substr(0, loginData.password.length() - 1);
            }
        )
    );

    LetterKeyPressedEvent::subscribe(
        new EventObserver<LetterKeyPressedEvent>(
            [&](LetterKeyPressedEvent* ev) {
                if (loginData.typingUsername)
                    loginData.username += ev->letter;
                else
                    loginData.password += ev->letter;
            }
        )
    );
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

std::pair<std::string, std::string> Player::getCredentials() const {
    return { loginData.username, picosha2::hash256_hex_string(loginData.tempsalt + picosha2::hash256_hex_string(loginData.permsalt + picosha2::hash256_hex_string(loginData.password))) };
}

std::pair<std::string, std::string> Player::getUserNamePw() const {
    std::string hiddenPassword = "";
    auto len = loginData.password.length();
    hiddenPassword.reserve(len);
    for (int i = 0; i < len; ++i)
        hiddenPassword += "*";
    return {loginData.username, hiddenPassword};
}

void Player::setSalts(std::string tempsalt, std::string permsalt) {
    loginData.tempsalt = tempsalt;
    loginData.permsalt = permsalt;
}

void Player::login() {
    JSON json;
    auto credentials = getCredentials();
    json["username"] = "'" + credentials.first + "'";
    json["passwordHash"] = "'" + credentials.second; +"'";
    socket->emit("login", json);
    loginData.password = "";
    loginData.username = "";
}

void Player::signUp() {
    JSON json;
    json["username"] = "'" + loginData.username + "'";
    json["passwordHash"] = "'" + picosha2::hash256_hex_string(loginData.password) +"'";
    socket->emit("sign up", json);
    loginData.password = "";
    loginData.username = "";
}

void Player::walk(VTile pos) {
    JSON json;
    json["x"] = std::to_string(pos.x);
    json["y"] = std::to_string(pos.y);
    json["id"] = std::to_string(id);
    socket->emit("walk", json);
}
