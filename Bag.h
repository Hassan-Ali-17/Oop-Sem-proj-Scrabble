#ifndef BAG_H
#define BAG_H

#include "Piece.h"

class Bag {
private:
    Piece* pieces[26];
public:
    Bag();
    ~Bag();
    Piece* getPiece(int index);
};

#endif // BAG_H