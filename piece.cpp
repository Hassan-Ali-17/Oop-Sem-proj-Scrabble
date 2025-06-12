#include "Piece.h"

Piece::Piece(int value, char letter, int amount) {
    this->value = value;
    this->letter = letter;
    this->amount = amount;
    this->remaining = amount;
}

int Piece::getValue() {
    return value;
}

char Piece::getLetter() {
    return letter;
}

int Piece::getAmount() {
    return amount;
}

int Piece::getRemaining() {
    return remaining;
}

void Piece::pieceOut() {
    remaining--;
}

void Piece::pieceIn() {
    remaining++;
}

void Piece::setSprite(std::string path) {
    pieceTexture.loadFromFile(path);
    pieceSprite.setTexture(pieceTexture);
}

void Piece::drawPiece(sf::RenderWindow& window, int x, int y) {
    pieceSprite.setPosition(x, y);
    window.draw(pieceSprite);
}

sf::FloatRect Piece::getGlobalBounds(int x, int y) {
    pieceSprite.setPosition(x, y);
    return pieceSprite.getGlobalBounds();
}