#include "../headers/RegisterState.hpp"
#include "../headers/Utility.hpp"
#include "../headers/ResourceHolder.hpp"
#include "../headers/player.hpp"
#include "../headers/event_macros.hpp"

#include <iostream>

using namespace std;
using namespace sf;

RegisterState::RegisterState(StateStack& stack, Context context)
: State(stack, context), 
realname  (*context.fonts, "Name"),
username  (*context.fonts, "Username"),
email     (*context.fonts, "Email"),
password1 (*context.fonts, "Password", true),
password2 (*context.fonts, "Confirm Password", true),
mAlphaText (new AlphaText (*context.fonts, "")),
registerButton ("Register",context.fonts->get(Fonts::Normal), 22),
backButton ("Back", context.fonts->get(Fonts::Normal), context.textures->get(Textures::smallButton))
{
    //USING TITLE FOR TESTING, MIGHT CHANGE TO LoginScreen LATTER!
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
    centerOrigin (mBackgroundSprite);

    mAlphaText->setPosition (300, 350);

    fields5.setTexture(context.textures->get(Textures::fields5));
    fields5.setPosition(340, 290);

    registerButton.setColor (LIGHT_BLUE);

    registerButton.setPosition(440, 573);
    backButton.setPosition(420, 616);

    realname.setPosition (364, 317);
    username.setPosition (364, 310 + 52);
    email.setPosition   (364, 310 + 102);
    password1.setPosition   (364, 310 + 150);
    password2.setPosition   (364, 310 + 197);

    realname.secretText = "";
    realname.adjustText();
    email.secretText = "";
    email.adjustText();
    password1.secretText = "";
    password1.adjustText();
    password2.secretText = "";
    password2.adjustText();

    inputBoxes = {&realname, &username, &email, &password1, &password2};
    selectedBox = 0;
    inputBoxes[selectedBox]->select();

    if (gameSave.readSuccess) {
        username.secretText = gameSave.name;
        username.adjustText();
        gameSave.password = "";
    }
}

void RegisterState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
    window.draw(fields5);
    window.draw(realname);
    window.draw(username);
    window.draw(email);
    window.draw(password1);
    window.draw(password2);
    window.draw(backButton);
    window.draw(registerButton);
    window.draw (*mAlphaText);
}

bool RegisterState::update(sf::Time dt)
{
    switch (getContext().net->receive()) {
        case packetID::RegisterResponse : {
            if (getContext().net->receiveRegisterResponse()) {
                cout << "Successfully Registered!\n";                
                mAlphaText->setString ("Successfully Registered", sf::Color::Green);
                gameSave.name = username.getText();
                gameSave.password = password1.getText(); 
                gameSave.save();
                *getContext().justRegistered = true;
                goToLogin();
            } else {
                cout << "Error on registration\n";
                mAlphaText->setString ("Error on registration");
            }
            break;    
        }
    }
    getContext().net->update();

    mAlphaText->update(dt, ignoreCommandQueue); 
    realname.update(dt, ignoreCommandQueue);
    username.update(dt, ignoreCommandQueue);
    email.update(dt, ignoreCommandQueue);
    password1.update(dt, ignoreCommandQueue);
    password2.update(dt, ignoreCommandQueue);

	return true;
}

bool RegisterState::handleEvent(const sf::Event& event)
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
            if        (MOUSE_BUTTON_ON(backButton)) {
                goToLogin(); 
            } else if (MOUSE_BUTTON_ON(registerButton)) {
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
            registration();        
        }

    inputBoxes[selectedBox]->handleEvent(event);
}

void RegisterState::goToHome() {
    requestStateClear();
    requestStackPush(States::Home);
}

void RegisterState::goToLogin() {
    requestStateClear();
    requestStackPush(States::Login);
}

void RegisterState::registration() {
    #define CHECKINPUT(INPUTX) if (INPUTX.getText() == "")
    CHECKINPUT (username) { mAlphaText->setString ("username not filled!"); return; }
    CHECKINPUT (realname) { mAlphaText->setString ("realname not filled!"); return; }
    CHECKINPUT (email)    { mAlphaText->setString ("email not filled!"   ); return; }
    CHECKINPUT (password1){ mAlphaText->setString ("first password field not filled!"); return; }
    CHECKINPUT (password2){ mAlphaText->setString ("second password field not filled!"); return; }

    if (password1.getText() != password2.getText()) { mAlphaText->setString ("Passwords do not match!"); return; }
    bool atNotFound = true;
    for (const auto& letter : email.getText()) { if (letter == '@') {atNotFound = false; break;} }
    if (atNotFound) { mAlphaText->setString ("not a valid email!"); return; }

    getContext().net->sendRegister(username.getText(), password1.getText(), realname.getText(), email.getText());
}