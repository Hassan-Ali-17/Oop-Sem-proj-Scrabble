#ifndef RACK_H
#define RACK_H

#include "Piece.h"
#include "Bag.h"
#include <ctime>
#include <random>
#include <chrono>

class Rack {
private:
    static int turn;
    Piece* pieces[7];
    int number;
    int filledAmount;
public:
    Rack(int number);
    ~Rack();
    void fillRack(Bag& bag);
    void drawRack(sf::RenderWindow& window, int startX, int startY);
    Piece* getTileAt(int index);
    void removeTileAt(int index);
    void addLetter(char letter, int value);
    int getNumber();
};

#endif // RACK_H