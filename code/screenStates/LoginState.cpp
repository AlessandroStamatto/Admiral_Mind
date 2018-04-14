#include "../headers/LoginState.hpp"
#include "../headers/Utility.hpp"
#include "../headers/ResourceHolder.hpp"
#include "../headers/player.hpp"
#include "../headers/event_macros.hpp"

#include <iostream>

using namespace std;
using namespace sf;

LoginState::LoginState(StateStack& stack, Context context)
: State(stack, context), 
username (*context.fonts, "Username"),
password (*context.fonts, "Password", true),
server (*context.fonts, "Server"),
mAlphaText (new AlphaText (*context.fonts, "")),
loginButton ("LOGIN",context.fonts->get(Fonts::Normal), 22),
registerButton ("Register", context.fonts->get(Fonts::Normal), context.textures->get(Textures::smallButton))
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
    centerOrigin (mBackgroundSprite);

    mAlphaText->setPosition (300, 350);

    loginFields.setTexture (context.textures->get(Textures::fields3));
    loginFields.setPosition (340,290);

    centerOrigin(loginButton);
    loginButton.setColor (LIGHT_BLUE);
    loginButton.setPosition(465, 486);

    registerButton.setPosition(414, 520);

    username.setPosition (360, 317);
    password.setPosition (360, 310 + 55);
    server.setPosition   (360, 310 + 102);

    username.secretText = "";
    password.secretText = "";
    server.secretText = "server.admiralmind.pairg.dimap.ufrn.br";

    if (gameSave.readSuccess) {
        username.secretText = gameSave.name;
        password.secretText = gameSave.password;
        server.secretText = gameSave.server;
    }

    username.adjustText();
    password.adjustText();
    server.adjustText();

    inputBoxes = {&username, &password, &server};
    inputBoxes[selectedBox]->select();
}

void LoginState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
    window.draw(loginFields);
    window.draw(username);
    window.draw(password);
    window.draw(server);
    window.draw(loginButton);
    window.draw(registerButton);
    window.draw (*mAlphaText);
}

bool LoginState::update(sf::Time dt)
{
    if (*getContext().justRegistered) {
        *getContext().justRegistered = false;
        mAlphaText->setString ("Successfully Registered!", sf::Color::Green);
    }

    switch (getContext().net->receive()) {
        case packetID::LoginResponse : {
            if (getContext().net->receiveLoginResponse()) {
                requestStateClear();
                requestStackPush(States::Home);
            } else {
                mAlphaText->setString ("Login incorrect");
            }
            break;
        }
        case packetID::RegisterResponse : {
            if (getContext().net->receiveRegisterResponse()) {
                cout << "Successfully Registered!\n";                
                mAlphaText->setString ("Successfully Registered", sf::Color::Green);
            } else {
                cout << "Error on registration\n";
                mAlphaText->setString ("Error on registration");
            }
            break;
        }
    }
    getContext().net->update();

    mAlphaText->update(dt, ignoreCommandQueue); 
    username.update(dt, ignoreCommandQueue);
    password.update(dt, ignoreCommandQueue);
    server.update(dt, ignoreCommandQueue);

	return true;
}

bool LoginState::handleEvent(const sf::Event& event)
{
    if ( MOUSE_RELEASED  )
        if (MOUSE_BUTTON(Left)) {
            for (int i = 0; i < inputBoxes.size(); ++i) {
                // sf::FloatRect ele = inputBoxes[i]->textSquare.getGlobalBounds();
                // cout << "Left:" << ele.left << " Top:" << ele.top << " Width:" << ele.width << " height:" << ele.height << "\n";
                if (inputBoxes[i]->contains(event.mouseButton.x, event.mouseButton.y)) {
                    inputBoxes[selectedBox]->unselect();
                    selectedBox = i;
                    inputBoxes[selectedBox]->select();
                }
            }
            if        (MOUSE_BUTTON_ON(loginButton)) {
                goToHome(); 
            }
            else if (MOUSE_BUTTON_ON(registerButton)) {
                registration();
            }
        }

    if ( KEY_RELEASED )
        if (ON_KEY(Up)) {   
            inputBoxes[selectedBox]->unselect();
            selectedBox -= 1;
            if (selectedBox < 0) selectedBox = inputBoxes.size() - 1;
            inputBoxes[selectedBox]->select();
        }
        else if (ON_KEY(Down) or ON_KEY(Tab)) {
            inputBoxes[selectedBox]->unselect();
            selectedBox += 1;
            selectedBox %= inputBoxes.size();
            inputBoxes[selectedBox]->select();
        }
        else if (ON_KEY(Return)) {
            goToHome();        
        }

    inputBoxes[selectedBox]->handleEvent(event);
}

void LoginState::goToHome() {
    mAlphaText->setString ("Connecting to server...", sf::Color::Black);
    if (not getContext().net->connect (server.getText())) {
        mAlphaText->setString ("Server not found");
        mAlphaText->delay();
        return;
    }
    mAlphaText->setString ("");
    getContext().player->name = username.getText();
    gameSave.name = username.getText();
    gameSave.password = password.secretText;
    gameSave.server = server.getText(); 
    gameSave.save();
    getContext().net->sendLogin(gameSave.name, gameSave.password);
}

void LoginState::registration() {
    mAlphaText->setString ("Connecting to server...", sf::Color::Black);
    if (not getContext().net->connect (server.getText())) {
        mAlphaText->setString ("Server not found");
        mAlphaText->delay();
        return;
    }
    mAlphaText->setString ("");
    gameSave.name = username.getText();
    gameSave.server = server.getText(); 
    gameSave.save();
    requestStateClear();
    requestStackPush(States::Register);
}