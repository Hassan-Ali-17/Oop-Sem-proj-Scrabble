#ifndef BOARD_H
#define BOARD_H

#include "Tile.h"

class Board {
private:
    Tile* tiles[15][15];
public:
    Board();
    ~Board();
    void draw(sf::RenderWindow& window);
    Tile* getTile(int i, int j);
    void getTemporaryTiles(Tile* tempTiles[], int& count);
    void clearTemporaryTiles();
    void confirmTemporaryTiles();
    bool getWords(std::string& horizontalWord, std::string& verticalWord, int row, int col);
    void getAllWords(std::string words[], int& count);
};

#endif 