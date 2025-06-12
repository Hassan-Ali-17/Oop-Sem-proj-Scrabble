#include "Bag.h"
#include <string>

Bag::Bag() {
    int values[26] = { 1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10 };
    int amounts[26] = { 9,2,2,4,12,2,3,2,9,1,1,4,2,6,8,2,1,6,4,6,4,2,2,1,2,1 };
    for (int i = 0; i < 26; i++) {
        pieces[i] = new Piece(values[i], 65 + i, amounts[i]);
        std::string p = "images/" + std::string(1, char(65 + i)) + ".png";
        pieces[i]->setSprite(p);
    }
}

Bag::~Bag() {
    for (int i = 0; i < 26; i++) {
        delete pieces[i];
    }
}

Piece* Bag::getPiece(int index) {
    if (index >= 0 && index < 26)
        return pieces[index];
    return nullptr;
}