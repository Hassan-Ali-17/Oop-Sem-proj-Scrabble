#include "Tile.h"
#include <SFML/Graphics.hpp>

Tile::Tile(int value, char letter, std::string state, int x, int y) : GameEntity(x, y) {
    this->value = value;
    this->letter = letter;
    this->state = state;
    this->isplaced = false;
    this->isTemporary = false;
    this->x = x;
    this->y = y;
    if (state == "Normal") {
        tileTexture.loadFromFile("images/normal.png");
        tileSprite.setTexture(tileTexture);
    }
    else if (state == "Double Letter") {
        tileTexture.loadFromFile("images/double letter.png");
        tileSprite.setTexture(tileTexture);
    }
    else if (state == "Triple Letter") {
        tileTexture.loadFromFile("images/triple letter.png");
        tileSprite.setTexture(tileTexture);
    }
    else if (state == "Double Word") {
        tileTexture.loadFromFile("images/double word.png");
        tileSprite.setTexture(tileTexture);
    }
    else if (state == "Triple Word") {
        tileTexture.loadFromFile("images/triple word.png");
        tileSprite.setTexture(tileTexture);
    }
    else if (state == "Start") {
        tileTexture.loadFromFile("images/Start.png");
        tileSprite.setTexture(tileTexture);
    }
}

void Tile::setValue(int value) {
    this->value = value;
}

void Tile::setLetter(char letter) {
    this->letter = letter;
}

void Tile::setState(std::string state) {
    this->state = state;
}

int Tile::getValue() {
    return value;
}

char Tile::getLetter() {
    return letter;
}

std::string Tile::getState() {
    return state;
}

bool Tile::getIsPlaced() {
    return isplaced;
}

void Tile::setIsPlaced(bool isplaced) {
    this->isplaced = isplaced;
}

bool Tile::getIsTemporary() {
    return isTemporary;
}

void Tile::setIsTemporary(bool temp) {
    this->isTemporary = temp;
}

void Tile::draw(sf::RenderWindow& window) {
    tileSprite.setPosition(x * 40, y * 40);
    window.draw(tileSprite);

    if (letter != ' ') {
        sf::Font font;
        font.loadFromFile("poppins-black.ttf");
        sf::Text text;
        text.setFont(font);
        text.setString(letter);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(x * 40 + 20, y * 40 + 20);

        window.draw(text);
    }
}

void Tile::draw(sf::RenderWindow& window, int x, int y) {
    tileSprite.setPosition(x, y);
    window.draw(tileSprite);

    if (letter != ' ') {
        sf::Font font;
        font.loadFromFile("poppins-black.ttf");
        sf::Text text;
        text.setFont(font);
        text.setString(letter);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(x + 20, y + 20);

        window.draw(text);
    }
}

int Tile::getX() {
    return x;
}

int Tile::getY() {
    return y;
}