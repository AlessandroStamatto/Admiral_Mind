#ifndef EVENT_MACROS_HPP
#define EVENT_MACROS_HPP

#define MOUSE_PRESSED event.type == sf::Event::MouseButtonPressed
#define MOUSE_BUTTON(Side) event.mouseButton.button == sf::Mouse::Side
#define MOUSE_RELEASED event.type == sf::Event::MouseButtonReleased
#define MOUSE_MOVED event.type == sf::Event::MouseMoved
#define MOUSE_MOVED_ON(element) element.getGlobalBounds().contains({static_cast<float> (event.mouseMove.x),static_cast<float> (event.mouseMove.y)})
#define MOUSE_BUTTON_ON(element) element.getGlobalBounds().contains({static_cast<float> (event.mouseButton.x), static_cast<float> (event.mouseButton.y)})
#define KEY_RELEASED event.type == sf::Event::KeyReleased
#define ON_KEY(XK) event.key.code == sf::Keyboard::XK
#define TEXT_ENTERED event.type == sf::Event::TextEntered
#define TEXT_KEY event.text.unicode
#define CONTAINS_MOUSE(element) element->contains(event.mouseButton.x, event.mouseButton.y)

#endif //EVENT_MACROS_HPP