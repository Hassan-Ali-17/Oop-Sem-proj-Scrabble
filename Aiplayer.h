#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "Dictionary.h"
#include "Bag.h"
#include "Board.h"
#include "Rack.h"
#include <string>

class AIPlayer {
private:
    Dictionary& dictionary;
    Bag& bag;
    static const int MAX_WORDS = 255;
    static const int MAX_WORD_LENGTH = 8;
    static constexpr const char* WORDS_PATH = "";
    static const int MAX_POSITIONS = 225; // 15 * 15 for board size
    char possibleWords[MAX_WORDS][MAX_WORD_LENGTH];
    int wordCount;

    void generateSubsets(char* letters, int letterCount, int len, int start, char* current, int currLen);
    bool placeWordAt(Board& board, Rack* rack, char* word, int len, int row, int col, bool horizontal);
    int getLetterValue(char letter);
public:
    AIPlayer(Dictionary& dict, Bag& b);
    void generateWords(Rack* rack);
    bool makeMove(Board* board, Rack* rack, std::string& attemptedWord, bool& isRandomWord, bool& needsApproval);
    bool makeRandomWord(Board* board, Rack* rack, std::string& randomWord, bool& isValid);
    bool placeRandomWord(Board* board, Rack* rack, const std::string& word);
};

#endif // AI_PLAYER_H