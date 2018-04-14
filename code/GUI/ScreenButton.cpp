#include "./../headers/Utility.hpp"
#include "./../headers/ScreenButton.hpp"

ScreenButton::ScreenButton (std::string str, const sf::Font& font, const sf::Texture& texture, int fontSize /*=22*/)
: sprite (texture), text (str, font, fontSize)
{
    text.setColor (DARK_BLUE);
    centerOrigin (text);
    auto bnds = sprite.getLocalBounds();
    text.setPosition(bnds.width/2, bnds.height/2 - 6);
}

void ScreenButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    auto currentTransform = getTransform() * states.transform;
    target.draw(sprite, currentTransform);
    target.draw(text, currentTransform);
}

sf::FloatRect ScreenButton::getGlobalBounds() const {
    return getTransform().transformRect(sprite.getGlobalBounds());
}
