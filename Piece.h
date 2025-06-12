#ifndef PIECE_H
#define PIECE_H

//#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <string>

class Piece {
private:
    int value;
    char letter;
    int amount;
    int remaining;
    sf::Texture pieceTexture;
    sf::Sprite pieceSprite;
public:
    Piece(int value, char letter, int amount);
    int getValue();
    char getLetter();
    int getAmount();
    int getRemaining();
    void pieceOut();
    void pieceIn();
    void setSprite(std::string path);
    void drawPiece(sf::RenderWindow& window, int x, int y);
    sf::FloatRect getGlobalBounds(int x, int y);
};

#endif // PIECE_H