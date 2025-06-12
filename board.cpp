#include "Board.h"
#include<string>

Board::Board() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            std::string state;
            if ((i == 7) && (j == 7))
                state = "Start";
            else if ((i == 0 && (j == 0 || j == 7 || j == 14)) ||
                (i == 7 && (j == 0 || j == 14)) ||
                (i == 14 && (j == 0 || j == 7 || j == 14))) {
                state = "Triple Word";
            }
            else if ((i == 1 && (j == 5 || j == 9)) ||
                (i == 5 && (j == 1 || j == 5 || j == 9 || j == 13)) ||
                (i == 9 && (j == 1 || j == 5 || j == 9 || j == 13)) ||
                (i == 13 && (j == 5 || j == 9))) {
                state = "Triple Letter";
            }
            else if ((i == 1 && (j == 1 || j == 13)) ||
                (i == 2 && (j == 2 || j == 12)) ||
                (i == 3 && (j == 3 || j == 11)) ||
                (i == 4 && (j == 4 || j == 10)) ||
                (i == 10 && (j == 4 || j == 10)) ||
                (i == 11 && (j == 3 || j == 11)) ||
                (i == 12 && (j == 2 || j == 12)) ||
                (i == 13 && (j == 1 || j == 13))) {
                state = "Double Word";
            }
            else if ((i == 0 && (j == 3 || j == 11)) ||
                (i == 2 && (j == 6 || j == 8)) ||
                (i == 3 && (j == 0 || j == 7 || j == 14)) ||
                (i == 6 && (j == 2 || j == 6 || j == 8 || j == 12)) ||
                (i == 8 && (j == 2 || j == 6 || j == 8 || j == 12)) ||
                (i == 11 && (j == 0 || j == 7 || j == 14)) ||
                (i == 12 && (j == 6 || j == 8)) ||
                (i == 14 && (j == 3 || j == 11))) {
                state = "Double Letter";
            }
            else {
                state = "Normal";
            }
            tiles[i][j] = new Tile(0, ' ', state, i, j);
        }
    }
}

Board::~Board() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            delete tiles[i][j];
        }
    }
}

void Board::draw(sf::RenderWindow& window) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            tiles[i][j]->draw(window, j * 40, i * 40);
        }
    }
}

Tile* Board::getTile(int i, int j) {
    if (i >= 0 && i < 15 && j >= 0 && j < 15)
        return tiles[i][j];
    return nullptr;
}

void Board::getTemporaryTiles(Tile* tempTiles[], int& count) {
    count = 0;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (tiles[i][j]->getIsTemporary()) {
                tempTiles[count++] = tiles[i][j];
            }
        }
    }
}

void Board::clearTemporaryTiles() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (tiles[i][j]->getIsTemporary()) {
                tiles[i][j]->setLetter(' ');
                tiles[i][j]->setValue(0);
                tiles[i][j]->setIsPlaced(false);
                tiles[i][j]->setIsTemporary(false);
            }
        }
    }
}

void Board::confirmTemporaryTiles() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (tiles[i][j]->getIsTemporary()) {
                tiles[i][j]->setIsTemporary(false);
            }
        }
    }
}

bool Board::getWords(std::string& horizontalWord, std::string& verticalWord, int row, int col) {
    bool foundWord = false;
    horizontalWord = "";
    verticalWord = "";

    int startCol = col;
    while (startCol > 0 && (tiles[row][startCol - 1]->getIsPlaced() || tiles[row][startCol - 1]->getIsTemporary())) {
        startCol--;
    }

    for (int j = startCol; j < 15; j++) {
        if (tiles[row][j]->getIsPlaced() || tiles[row][j]->getIsTemporary()) {
            horizontalWord += tiles[row][j]->getLetter();
        }
        else {
            break;
        }
    }

    if (horizontalWord.length() > 1) {
        foundWord = true;
    }

    int startRow = row;
    while (startRow > 0 && (tiles[startRow - 1][col]->getIsPlaced() || tiles[startRow - 1][col]->getIsTemporary())) {
        startRow--;
    }

    for (int i = startRow; i < 15; i++) {
        if (tiles[i][col]->getIsPlaced() || tiles[i][col]->getIsTemporary()) {
            verticalWord += tiles[i][col]->getLetter();
        }
        else {
            break;
        }
    }

    if (verticalWord.length() > 1) {
        foundWord = true;
    }

    return foundWord;
}

void Board::getAllWords(std::string words[], int& count) {
    count = 0;
    bool checked[15][15] = { false };

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (tiles[i][j]->getIsTemporary() && !checked[i][j]) {
                int startJ = j;
                while (startJ > 0 && (tiles[i][startJ - 1]->getIsPlaced() || tiles[i][startJ - 1]->getIsTemporary())) {
                    startJ--;
                }

                std::string word = "";
                int k = startJ;
                while (k < 15 && (tiles[i][k]->getIsPlaced() || tiles[i][k]->getIsTemporary())) {
                    word += tiles[i][k]->getLetter();
                    checked[i][k] = true;
                    k++;
                }

                if (word.length() > 1) {
                    words[count++] = word;
                }
            }
        }
    }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            checked[i][j] = false;
        }
    }

    for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 15; i++) {
            if (tiles[i][j]->getIsTemporary() && !checked[i][j]) {
                int startI = i;
                while (startI > 0 && (tiles[startI - 1][j]->getIsPlaced() || tiles[startI - 1][j]->getIsTemporary())) {
                    startI--;
                }
                std::string word = "";
                int k = startI;
                while (k < 15 && (tiles[k][j]->getIsPlaced() || tiles[k][j]->getIsTemporary())) {
                    word += tiles[k][j]->getLetter();
                    checked[k][j] = true;
                    k++;
                }

                if (word.length() > 1) {
                    words[count++] = word;
                }
            }
        }
    }
}