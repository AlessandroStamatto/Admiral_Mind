#ifndef ALPHATEXT_HPP
#define ALPHATEXT_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "colorDefines.hpp"

#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Font.hpp"


class AlphaText : public SceneNode
{
	public:
		explicit			AlphaText(const FontHolder& fonts, const std::string& text, uint8_t tm = 4);

		void				setString(const std::string& text, sf::Color color = sf::Color::Red);
                void                            setFont(Fonts::ID font);
                void                            delay  ();
                std::string                     getText ();

	private:
		virtual void	    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
                virtual void        updateCurrent(sf::Time dt, CommandQueue& ignore);

	public:
        sf::Text            mText;
        const FontHolder&   mFonts;
        uint8_t             timeToFade;
        uint8_t             alphaPerSecond;
        double              totalTime;
        sf::Color           mColor;
        double              timeDelay;
};

#endif // ALPHATEXT_HPP
