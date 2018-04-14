#include "../headers/TextNode.hpp"
#include "../headers/Utility.hpp"

#include "SFML/Graphics/RenderTarget.hpp"

    
TextNode::TextNode(const FontHolder& fonts, const std::string& text)
: mFonts (fonts)
{
	mText.setFont(fonts.get(Fonts::Normal));
	mText.setCharacterSize(20);
	setString(text);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mText, states);
}

void TextNode::setString(const std::string& text)
{
	mText.setString(text);
	// centerOrigin(mText);
}

void TextNode::setFont(Fonts::ID font)
{
    mText.setFont(mFonts.get(font));
}

bool TextNode::contains (int mx, int my) { //It would be best to move this method to the parent SceneNode class
    return (static_cast<sf::IntRect> (getTransform().transformRect(mText.getGlobalBounds()))).contains(mx, my);
}