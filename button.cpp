#include "Button.h"

Button::Button(float x, float y, float width, float height, sf::Font& font, std::string text) {
    shape.setPosition(sf::Vector2f(x, y));
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(100, 100, 100));
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);

    this->text.setFont(font);
    this->text.setString(text);
    this->text.setCharacterSize(18);
    this->text.setFillColor(sf::Color::White);

    sf::FloatRect textRect = this->text.getLocalBounds();
    this->text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    this->text.setPosition(x + width / 2.0f, y + height / 2.0f);
}

bool Button::isMouseOver(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    return shape.getGlobalBounds().contains(mousePosF);
}

void Button::setBackColor(sf::Color color) {
    shape.setFillColor(color);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}