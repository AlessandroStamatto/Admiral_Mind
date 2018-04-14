#include "../headers/AlphaText.hpp"
#include "../headers/Utility.hpp"

#include "SFML/Graphics/RenderTarget.hpp"
#include <iostream>
#include <algorithm>

using namespace std;
    
AlphaText::AlphaText(const FontHolder& fonts, const std::string& text, uint8_t tm /*=4*/)
: mFonts (fonts), timeToFade(tm), alphaPerSecond(255/timeToFade), totalTime(0)
, timeDelay (0)
{
	mText.setFont(fonts.get(Fonts::Normal));
	mText.setCharacterSize(54);
	setString(text);
    mColor = sf::Color::Red;
    mText.setColor (mColor);
}

void AlphaText::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mText, states);
}

void AlphaText::setString(const std::string& text, sf::Color color)
{
	mText.setString(text);
	// centerOrigin(mText);
    mColor = color;
    mColor.a = 255;
    totalTime = 0;
    timeDelay = 0;
}

std::string AlphaText::getText () {
    return mText.getString();
}

void AlphaText::setFont(Fonts::ID font)
{
    mText.setFont(mFonts.get(font));
}

void AlphaText::delay ()
{
    timeDelay = 2;
}

void AlphaText::updateCurrent(sf::Time dt, CommandQueue& ignore) {
    if (timeDelay > 0) {
        timeDelay -= dt.asSeconds();
        return;
    } 
    if (totalTime < timeToFade) {
        uint8_t a = mColor.a - alphaPerSecond * dt.asSeconds();
        mColor.a  = std::min (mColor.a, a);
        mText.setColor (mColor);
        totalTime += dt.asSeconds();
    }
}