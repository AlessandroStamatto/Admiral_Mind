#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "GameDatatypes.hpp"

#include "SFML/Window/Keyboard.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics.hpp"

#include <sstream>


namespace sf
{
	class Sprite;
	class Text;
}

std::ostream& operator<<(std::ostream& cout, Ship ship);

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string		toString(const T& value);

// Convert enumerators to strings
std::string		toString(sf::Keyboard::Key key);

// Call setOrigin() with the center of the object
void            centerOrigin(sf::RectangleShape& shape);
void			centerOrigin(sf::Sprite& sprite);
void			centerOrigin(sf::Text& text);

// Degree/radian conversion
float			toDegree(float radian);
float			toRadian(float degree);

// Random number generation
int				randomInt(int exclusiveMax);

// Vector operations
float			length(sf::Vector2f vector);
sf::Vector2f	unitVector(sf::Vector2f vector);


#include "Utility.inl"
#endif // UTILITY_HPP
