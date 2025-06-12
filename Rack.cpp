#include "Rack.h"
#include <string>

int Rack::turn = 1;

Rack::Rack(int number) {
    this->number = number;
    this->filledAmount = 0;
    for (int i = 0; i < 7; i++) {
        pieces[i] = nullptr;
    }
    srand(time(0));
}

void Rack::fillRack(Bag& bag) {
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(0, 25);

    for (int i = 0; i < 7; ++i) {
        if (pieces[i] == nullptr) {
            int tries = 0;
            while (tries < 50) {
                int index = dist(rng);
                Piece* fromBag = bag.getPiece(index);
                if (fromBag && fromBag->getRemaining() > 0) {
                    pieces[i] = new Piece(fromBag->getValue(), fromBag->getLetter(), 1);
                    std::string path = "images/" + std::string(1, fromBag->getLetter()) + ".png";
                    pieces[i]->setSprite(path);
                    fromBag->pieceOut();
                    filledAmount++;
                    break;
                }
                tries++;
            }
        }
    }
}

void Rack::drawRack(sf::RenderWindow& window, int startX, int startY) {
    for (int i = 0; i < 7; ++i) {
        if (pieces[i]) {
            pieces[i]->drawPiece(window, startX + i * 50, startY);
        }
    }
}

Piece* Rack::getTileAt(int index) {
    if (index >= 0 && index < 7)
        return pieces[index];
    return nullptr;
}

void Rack::removeTileAt(int index) {
    if (index >= 0 && index < 7 && pieces[index]) {
        delete pieces[index];
        pieces[index] = nullptr;
        filledAmount--;
    }
}

void Rack::addLetter(char letter, int value) {
    for (int i = 0; i < 7; i++) {
        if (pieces[i] == nullptr) {
            pieces[i] = new Piece(value, letter, 1);
            std::string path = "images/" + std::string(1, letter) + ".png";
            pieces[i]->setSprite(path);
            filledAmount++;
            break;
        }
    }
}

int Rack::getNumber() {
    return number;
}

Rack::~Rack() {
    for (int i = 0; i < 7; ++i) {
        delete pieces[i];
    }
}