#ifndef TILE_H
#define TILE_H

#include "GameEntity.h"
#include "Drawable.h"
#include <string>

class Tile : public GameEntity, public Drawable {
private:
    int value;
    char letter;
    std::string state;
    bool isplaced;
    bool isTemporary;
    sf::Texture tileTexture;
    sf::Sprite tileSprite;
    int x;
    int y;
public:
    Tile(int value, char letter, std::string state, int x, int y);
    void setValue(int value);
    void setLetter(char letter);
    void setState(std::string state);
    int getValue();
    char getLetter();
    std::string getState();
    bool getIsPlaced();
    void setIsPlaced(bool isplaced);
    bool getIsTemporary();
    void setIsTemporary(bool temp);
    void draw(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window, int x, int y);
    int getX();
    int getY();
};

#endif // TILE_H