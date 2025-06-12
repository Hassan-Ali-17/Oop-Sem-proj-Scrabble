#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;
public:
    Button(float x, float y, float width, float height, sf::Font& font, std::string text);
    bool isMouseOver(sf::RenderWindow& window);
    void setBackColor(sf::Color color);
    void draw(sf::RenderWindow& window);
};

#endif // BUTTON_H