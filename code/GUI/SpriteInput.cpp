#include "./../headers/Utility.hpp"
#include "./../headers/SpriteInput.hpp"

#include <iostream>
#include <algorithm>

#include "../headers/event_macros.hpp"

using namespace std;
using namespace sf;

SpriteInput::SpriteInput (const FontHolder& fonts, sf::String lab, bool text_is_secret)
: mFonts(fonts), textSquare({400.0, 40.0}),
secret(text_is_secret), cursor ({3, 18}), color(Color::Black)
{
    transform(begin(lab), end(lab), begin(lab), ::toupper);
    label = sf::Text (lab, mFonts.get(Fonts::Normal), 22);
    label.setColor(DARK_BLUE);
    label.setPosition (0, -14);

    text = sf::Text ("", mFonts.get(Fonts::Normal), 22);
    text.setColor(LIGHT_BLUE);
    if (text_is_secret) text.setPosition(5, 15);
    else                text.setPosition(5, 10);

    secretText = "";

    cursor.setFillColor(color);
}

bool SpriteInput::contains (int mx, int my) {
    return (static_cast<sf::IntRect> (getTransform().transformRect(textSquare.getGlobalBounds()))).contains(mx, my);
}

void SpriteInput::select () {
    textSquare.setOutlineColor(Color::Red);
    textSquare.setOutlineThickness(5);
    selected = true;
}

void SpriteInput::unselect () {
    textSquare.setOutlineColor(Color::Black);
    textSquare.setOutlineThickness(4);
    selected = false;
}

std::string SpriteInput::getText() {
    return secretText;
}

void SpriteInput::adjustText() {
    if (not secret) {
        if (secretText.size() > 26)    
            text.setString (secretText.substr(secretText.size() - 27, 27));
        else
            text.setString (secretText);
    } else {
        if (secretText.size() > 34)
            text.setString (std::string (35, '*'));
        else
            text.setString (std::string (secretText.size(), '*'));
    }

    int width = text.getLocalBounds().width;
    if (secret)
        cursor.setPosition (width + 10, 14);
    else
        cursor.setPosition (width + 10, 15);
}

void SpriteInput::handleEvent(const sf::Event& event) {
    if (TEXT_ENTERED) {
        if (TEXT_KEY == 8) {
            if (secretText.size() < 1) return;
            secretText.erase(secretText.size() - 1);
        }
        else if (TEXT_KEY > 31 and secretText.size() < 52)
            secretText += static_cast<char>(TEXT_KEY);
        adjustText();
    }
}

void SpriteInput::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw (text, states);
    target.draw (label, states);
    if (selected)
        target.draw (cursor, states);
}

void SpriteInput::updateCurrent(sf::Time dt, CommandQueue& commands) {
    int toChange = 255.0 * (dt.asSeconds() * 2);
    int finalA;
    if (increasing) {
        finalA = static_cast<int> (color.a) + toChange;
        if (finalA > 255) {
            finalA = 255;
            increasing = false;
        }
    } else {
        finalA = static_cast<int> (color.a) - toChange;
        if (finalA < 0) {
            finalA = 0;
            increasing = true;
        }
    }
    color.a = static_cast<uint8_t> (finalA);
    cursor.setFillColor(color);
}