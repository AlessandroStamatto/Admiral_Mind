#ifndef TEXTNODE_HPP
#define TEXTNODE_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "colorDefines.hpp"

#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Font.hpp"


class TextNode : public SceneNode
{
	public:
		explicit			TextNode(const FontHolder& fonts, const std::string& text);

		void				setString(const std::string& text);
        void                setFont(Fonts::ID font);
        bool                contains (int mx, int my);


	private:
		virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	public:
        sf::Text            mText;
        const FontHolder&   mFonts;
};

#endif // TEXTNODE_HPP
