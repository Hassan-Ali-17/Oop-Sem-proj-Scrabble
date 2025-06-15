#include "AIPlayer.h"
#include <chrono>
#include <random>

const int AIPlayer::MAX_WORD_LENGTH;
const int AIPlayer::MAX_POSITIONS; 

AIPlayer::AIPlayer(Dictionary& dict, Bag& b) : dictionary(dict), bag(b), wordCount(0) {}

template<typename T>
void customShuffle(T* arr, int size, std::mt19937& rng) {
    for (int i = size - 1; i > 0; --i) {
        std::uniform_int_distribution<int> dist(0, i);
        int j = dist(rng);
        T temp;
        if constexpr (std::is_array_v<T>) {
            for (size_t k = 0; k < std::extent_v<T>; ++k) {
                temp[k] = arr[i][k];
            }
            for (size_t k = 0; k < std::extent_v<T>; ++k) {
                arr[i][k] = arr[j][k];
            }
            for (size_t k = 0; k < std::extent_v<T>; ++k) {
                arr[j][k] = temp[k];
            }
        }
        else {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
}

bool AIPlayer::makeRandomWord(Board* board, Rack* rack, std::string& randomWord, bool& isValid) {
    char letters[8] = { 0 };
    int letterCount = 0;
    for (int i = 0; i < 7; ++i) {
        Piece* p = rack->getTileAt(i);
        if (p) letters[letterCount++] = p->getLetter();
    }

    if (letterCount < 2) return false;

    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> lenDist(2, std::min(4, letterCount));
    std::uniform_int_distribution<int> letterDist(0, letterCount - 1);

    int wordLen = lenDist(rng);
    randomWord = "";

    char availableLetters[8];
    for (int i = 0; i < letterCount; i++) {
        availableLetters[i] = letters[i];
    }
    int available = letterCount;

    for (int i = 0; i < wordLen; i++) {
        int index = letterDist(rng) % available;
        randomWord += availableLetters[index];
        for (int j = index; j < available - 1; j++) {
            availableLetters[j] = availableLetters[j + 1];
        }
        available--;
        letterDist = std::uniform_int_distribution<int>(0, std::max(0, available - 1));
    }

    isValid = dictionary.isValidWord(randomWord);
    return placeRandomWord(board, rack, randomWord);
}

bool AIPlayer::placeRandomWord(Board* board, Rack* rack, const std::string& word) {
    int len = word.length();
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    int positions[MAX_POSITIONS][2];
    int posCount = 0;

    bool isBoardEmpty = true;
    for (int i = 0; i < 15 && isBoardEmpty; ++i) {
        for (int j = 0; j < 15; ++j) {
            if (board->getTile(i, j)->getIsPlaced()) {
                isBoardEmpty = false;
                break;
            }
        }
    }

    if (isBoardEmpty) {
        for (int col = 7 - len + 1; col <= 7; ++col) {
            if (posCount < MAX_POSITIONS) {
                positions[posCount][0] = 7;
                positions[posCount][1] = col;
                posCount++;
            }
        }
        for (int row = 7 - len + 1; row <= 7; ++row) {
            if (posCount < MAX_POSITIONS) {
                positions[posCount][0] = row;
                positions[posCount][1] = 7;
                posCount++;
            }
        }
    }
    else {
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                if (j + len <= 15) {
                    bool valid = true;
                    for (int k = 0; k < len; ++k) {
                        Tile* tile = board->getTile(i, j + k);
                        if (tile->getIsPlaced() && tile->getLetter() != word[k]) {
                            valid = false;
                            break;
                        }
                    }
                    if (valid && posCount < MAX_POSITIONS) {
                        positions[posCount][0] = i;
                        positions[posCount][1] = j;
                        posCount++;
                    }
                }
                if (i + len <= 15) {
                    bool valid = true;
                    for (int k = 0; k < len; ++k) {
                        Tile* tile = board->getTile(i + k, j);
                        if (tile->getIsPlaced() && tile->getLetter() != word[k]) {
                            valid = false;
                            break;
                        }
                    }
                    if (valid && posCount < MAX_POSITIONS) {
                        positions[posCount][0] = i;
                        positions[posCount][1] = j;
                        posCount++;
                    }
                }
            }
        }
    }

    customShuffle(positions, posCount, rng);

    for (int i = 0; i < posCount; ++i) {
        int row = positions[i][0];
        int col = positions[i][1];

        if (col + len <= 15 && placeWordAt(*board, rack, const_cast<char*>(word.c_str()), len, row, col, true)) {
            return true;
        }
        if (row + len <= 15 && placeWordAt(*board, rack, const_cast<char*>(word.c_str()), len, row, col, false)) {
            return true;
        }
    }

    return false;
}

void AIPlayer::generateWords(Rack* rack) {
    wordCount = 0;
    char letters[8] = { 0 };
    int letterCount = 0;
    for (int i = 0; i < 7; ++i) {
        Piece* p = rack->getTileAt(i);
        if (p) letters[letterCount++] = p->getLetter();
    }

    char current[MAX_WORD_LENGTH] = { 0 };
    for (int len = 1; len <= letterCount; ++len) {
        generateSubsets(letters, letterCount, len, 0, current, 0);
    }
}

bool AIPlayer::makeMove(Board* board, Rack* rack, std::string& attemptedWord, bool& isRandomWord, bool& needsApproval) {
    isRandomWord = false;
    needsApproval = false;
    attemptedWord = "";

    generateWords(rack);

    if (wordCount > 0) {
        bool isBoardEmpty = true;
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                if (board->getTile(i, j)->getIsPlaced()) {
                    isBoardEmpty = false;
                    break;
                }
            }
            if (!isBoardEmpty) break;
        }

        static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        int wordIndices[MAX_WORDS];
        for (int i = 0; i < wordCount; ++i) wordIndices[i] = i;
        customShuffle(wordIndices, wordCount, rng);

        for (int w = 0; w < wordCount; ++w) {
            char* word = possibleWords[wordIndices[w]];
            int len = 0;
            while (word[len] != '\0') ++len;
            if (len < 2) continue;

            if (isBoardEmpty) {
                if (len <= 7 && placeWordAt(*board, rack, word, len, 7, 8 - len, true)) {
                    return true;
                }
                if (len <= 7 && placeWordAt(*board, rack, word, len, 8 - len, 7, false)) {
                    return true;
                }
            }
            else {
                for (int i = 0; i < 15; ++i) {
                    for (int j = 0; j < 15; ++j) {
                        if (j + len <= 15 && placeWordAt(*board, rack, word, len, i, j, true)) {
                            return true;
                        }
                        if (i + len <= 15 && placeWordAt(*board, rack, word, len, i, j, false)) {
                            return true;
                        }
                    }
                }
            }
        }
    }

    bool isValid;
    if (makeRandomWord(board, rack, attemptedWord, isValid)) {
        isRandomWord = true;
        needsApproval = !isValid;
        return true;
    }

    return false;
}

void AIPlayer::generateSubsets(char* letters, int letterCount, int len, int start, char* current, int currLen) {
    if (currLen == len) {
        current[currLen] = '\0';
        if (wordCount < MAX_WORDS && dictionary.isValidWord(current)) {
            for (int i = 0; i <= currLen; ++i) {
                possibleWords[wordCount][i] = current[i];
            }
            wordCount++;
        }
        return;
    }
    for (int i = start; i < letterCount; ++i) {
        current[currLen] = letters[i];
        generateSubsets(letters, letterCount, len, i + 1, current, currLen + 1);
    }
}

bool AIPlayer::placeWordAt(Board& board, Rack* rack, char* word, int len, int row, int col, bool horizontal) {
    if (horizontal) {
        if (col < 0 || col + len > 15 || row < 0 || row >= 15) return false;
    }
    else {
        if (row < 0 || row + len > 15 || col < 0 || col >= 15) return false;
    }

    for (int i = 0; i < len; ++i) {
        int r = horizontal ? row : row + i;
        int c = horizontal ? col + i : col;
        Tile* tile = board.getTile(r, c);
        if (!tile) return false;

        if (tile->getIsPlaced() && tile->getLetter() != word[i]) {
            return false;
        }
    }

    bool connects = false;
    bool isBoardEmpty = true;
    for (int i = 0; i < 15 && isBoardEmpty; ++i) {
        for (int j = 0; j < 15; ++j) {
            if (board.getTile(i, j)->getIsPlaced()) {
                isBoardEmpty = false;
                break;
            }
        }
    }

    if (isBoardEmpty) {
        bool goesThroughCenter = false;
        for (int i = 0; i < len; ++i) {
            int r = horizontal ? row : row + i;
            int c = horizontal ? col + i : col;
            if (r == 7 && c == 7) {
                goesThroughCenter = true;
                break;
            }
        }
        if (!goesThroughCenter) return false;
        connects = true;
    }
    else {
        for (int i = 0; i < len; ++i) {
            int r = horizontal ? row : row + i;
            int c = horizontal ? col + i : col;
            Tile* tile = board.getTile(r, c);

            if (tile->getIsPlaced()) {
                connects = true;
                continue;
            }

            if (r > 0 && board.getTile(r - 1, c)->getIsPlaced()) connects = true;
            if (r < 14 && board.getTile(r + 1, c)->getIsPlaced()) connects = true;
            if (c > 0 && board.getTile(r, c - 1)->getIsPlaced()) connects = true;
            if (c < 14 && board.getTile(r, c + 1)->getIsPlaced()) connects = true;
        }
        if (!connects) return false;
    }

    char rackLetters[8] = { 0 };
    int letterCount = 0;
    for (int i = 0; i < 7; ++i) {
        Piece* p = rack->getTileAt(i);
        if (p) rackLetters[letterCount++] = p->getLetter();
    }

    char tempRackLetters[8];
    for (int i = 0; i < letterCount; ++i) {
        tempRackLetters[i] = rackLetters[i];
    }

    for (int i = 0; i < len; ++i) {
        int r = horizontal ? row : row + i;
        int c = horizontal ? col + i : col;
        Tile* tile = board.getTile(r, c);

        if (tile->getIsPlaced() && tile->getLetter() == word[i]) {
            continue;
        }

        bool found = false;
        for (int j = 0; j < letterCount; ++j) {
            if (tempRackLetters[j] == word[i]) {
                tempRackLetters[j] = '\0';
                found = true;
                break;
            }
        }
        if (!found) return false;
    }

    for (int i = 0; i < len; ++i) {
        int r = horizontal ? row : row + i;
        int c = horizontal ? col + i : col;
        Tile* tile = board.getTile(r, c);

        if (tile->getIsPlaced()) continue;

        tile->setLetter(word[i]);
        tile->setValue(getLetterValue(word[i]));
        tile->setIsPlaced(true);
        tile->setIsTemporary(true);

        for (int k = 0; k < 7; ++k) {
            Piece* p = rack->getTileAt(k);
            if (p && p->getLetter() == word[i]) {
                rack->removeTileAt(k);
                break;
            }
        }
    }
    return true;
}

int AIPlayer::getLetterValue(char letter) {
    int values[26] = { 1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10 };
    return values[letter - 'A'];
}
