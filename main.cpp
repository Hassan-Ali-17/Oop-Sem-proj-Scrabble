#include "Board.h"
#include "Bag.h"
#include "Rack.h"
#include "Dictionary.h"
#include "AIPlayer.h"
#include "Button.h"
#include "YesNoDialog.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include<sstream>

int main() {
    Board board;
    Bag bag;
    sf::RenderWindow window(sf::VideoMode(1100, 600), "Scrabble Game");
    window.setFramerateLimit(60);
    sf::Event event;
    Rack* racks[2]; 
    Dictionary dictionary("dictionary.txt");
    AIPlayer ai(dictionary, bag);

    int selectedRackIndex = -1;
    int selectedRow = -1;
    int selectedCol = -1;
    int activeRack = 0;
    int scores[2] = { 0, 0 };

    sf::Font font;
    if (!font.loadFromFile("poppins-black.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return -1;
    }
    Button placeButton(700, 550, 100, 40, font, "Place");
    Button cancelButton(825, 550, 100, 40, font, "Cancel");
    YesNoDialog yesNoDialog(font);

    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(18);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(625, 100);
    statusText.setString("Select a tile on the board");

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(16);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(650, 50);
    scoreText.setString("Player: 0 | AI: 0");

    racks[0] = new Rack(1); 
    racks[1] = new Rack(2);
    racks[0]->fillRack(bag);
    racks[1]->fillRack(bag);

    bool waitingForAIWordApproval = false;
    std::string aiAttemptedWord = "";

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (activeRack == 0) { 
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    if (yesNoDialog.isVisible()) {
                        if (yesNoDialog.isYesClicked(window)) {
                            std::string invalidWords = yesNoDialog.getInvalidWords();
                            std::stringstream ss(invalidWords);
                            std::string word;
                            while (ss >> word) {
                                dictionary.addWordToDictionary(word);
                            }

                            Tile* tempTiles[225];
                            int tempCount = 0;
                            board.getTemporaryTiles(tempTiles, tempCount);

                            int moveScore = 0;
                            std::string words[50];
                            int wordCount = 0;
                            board.getAllWords(words, wordCount);

                            for (int w = 0; w < wordCount; w++) {
                                int wordScore = 0;
                                int wordMultiplier = 1;
                                for (int i = 0; i < 15; i++) {
                                    for (int j = 0; j < 15; j++) {
                                        if (board.getTile(i, j)->getLetter() == words[w][0]) {
                                            bool foundHorizontal = true;
                                            for (int k = 0; words[w][k] != '\0'; k++) {
                                                if (j + k >= 15 || board.getTile(i, j + k)->getLetter() != words[w][k]) {
                                                    foundHorizontal = false;
                                                    break;
                                                }
                                            }

                                            bool foundVertical = true;
                                            for (int k = 0; words[w][k] != '\0'; k++) {
                                                if (i + k >= 15 || board.getTile(i + k, j)->getLetter() != words[w][k]) {
                                                    foundVertical = false;
                                                    break;
                                                }
                                            }

                                            if (foundHorizontal) {
                                                for (int k = 0; words[w][k] != '\0'; k++) {
                                                    Tile* currentTile = board.getTile(i, j + k);
                                                    int letterValue = currentTile->getValue();
                                                    if (currentTile->getIsTemporary()) {
                                                        if (currentTile->getState() == "Double Letter") {
                                                            letterValue *= 2;
                                                        }
                                                        else if (currentTile->getState() == "Triple Letter") {
                                                            letterValue *= 3;
                                                        }
                                                        if (currentTile->getState() == "Double Word") {
                                                            wordMultiplier *= 2;
                                                        }
                                                        else if (currentTile->getState() == "Triple Word") {
                                                            wordMultiplier *= 3;
                                                        }
                                                    }
                                                    wordScore += letterValue;
                                                }
                                                break;
                                            }
                                            else if (foundVertical) {
                                                for (int k = 0; words[w][k] != '\0'; k++) {
                                                    Tile* currentTile = board.getTile(i + k, j);
                                                    int letterValue = currentTile->getValue();
                                                    if (currentTile->getIsTemporary()) {
                                                        if (currentTile->getState() == "Double Letter") {
                                                            letterValue *= 2;
                                                        }
                                                        else if (currentTile->getState() == "Triple Letter") {
                                                            letterValue *= 3;
                                                        }
                                                        if (currentTile->getState() == "Double Word") {
                                                            wordMultiplier *= 2;
                                                        }
                                                        else if (currentTile->getState() == "Triple Word") {
                                                            wordMultiplier *= 3;
                                                        }
                                                    }
                                                    wordScore += letterValue;
                                                }
                                                break;
                                            }
                                        }
                                    }
                                }
                                wordScore *= wordMultiplier;
                                moveScore += wordScore;
                            }
                            scores[activeRack] += moveScore;

                            board.confirmTemporaryTiles();
                            statusText.setString("Words added to dictionary! +" + std::to_string(moveScore) + " points");
                            scoreText.setString("Player: " + std::to_string(scores[0]) + " | AI: " + std::to_string(scores[1]));
                            racks[activeRack]->fillRack(bag);
                            activeRack = 1;
                            yesNoDialog.hide();
                        }
                        else if (yesNoDialog.isNoClicked(window)) {
                            Tile* tempTiles[225];
                            int tempCount = 0;
                            board.getTemporaryTiles(tempTiles, tempCount);

                            for (int i = 0; i < tempCount; i++) {
                                char letter = tempTiles[i]->getLetter();
                                int value = tempTiles[i]->getValue();
                                racks[activeRack]->addLetter(letter, value);
                            }

                            board.clearTemporaryTiles();
                            statusText.setString("Move canceled. Try again.");
                            yesNoDialog.hide();
                        }
                    }
                    else if (placeButton.isMouseOver(window)) {
                        Tile* tempTiles[225];
                        int tempCount = 0;
                        board.getTemporaryTiles(tempTiles, tempCount);

                        if (tempCount > 0) {
                            std::string words[50];
                            int wordCount = 0;
                            board.getAllWords(words, wordCount);

                            bool allValid = true;
                            std::string invalidWords = "";
                            for (int i = 0; i < wordCount; i++) {
                                if (!dictionary.isValidWord(words[i])) {
                                    allValid = false;
                                    invalidWords += std::string(words[i]) + " ";
                                }
                            }

                            if (allValid) {
                                int moveScore = 0;
                                for (int w = 0; w < wordCount; w++) {
                                    int wordScore = 0;
                                    int wordMultiplier = 1;
                                    for (int i = 0; i < 15; i++) {
                                        for (int j = 0; j < 15; j++) {
                                            if (board.getTile(i, j)->getLetter() == words[w][0]) {
                                                bool foundHorizontal = true;
                                                for (int k = 0; words[w][k] != '\0'; k++) {
                                                    if (j + k >= 15 || board.getTile(i, j + k)->getLetter() != words[w][k]) {
                                                        foundHorizontal = false;
                                                        break;
                                                    }
                                                }

                                                bool foundVertical = true;
                                                for (int k = 0; words[w][k] != '\0'; k++) {
                                                    if (i + k >= 15 || board.getTile(i + k, j)->getLetter() != words[w][k]) {
                                                        foundVertical = false;
                                                        break;
                                                    }
                                                }

                                                if (foundHorizontal) {
                                                    for (int k = 0; words[w][k] != '\0'; k++) {
                                                        Tile* currentTile = board.getTile(i, j + k);
                                                        int letterValue = currentTile->getValue();
                                                        if (currentTile->getIsTemporary()) {
                                                            if (currentTile->getState() == "Double Letter") {
                                                                letterValue *= 2;
                                                            }
                                                            else if (currentTile->getState() == "Triple Letter") {
                                                                letterValue *= 3;
                                                            }
                                                            if (currentTile->getState() == "Double Word") {
                                                                wordMultiplier *= 2;
                                                            }
                                                            else if (currentTile->getState() == "Triple Word") {
                                                                wordMultiplier *= 3;
                                                            }
                                                        }
                                                        wordScore += letterValue;
                                                    }
                                                    break;
                                                }
                                                else if (foundVertical) {
                                                    for (int k = 0; words[w][k] != '\0'; k++) {
                                                        Tile* currentTile = board.getTile(i + k, j);
                                                        int letterValue = currentTile->getValue();
                                                        if (currentTile->getIsTemporary()) {
                                                            if (currentTile->getState() == "Double Letter") {
                                                                letterValue *= 2;
                                                            }
                                                            else if (currentTile->getState() == "Triple Letter") {
                                                                letterValue *= 3;
                                                            }
                                                            if (currentTile->getState() == "Double Word") {
                                                                wordMultiplier *= 2;
                                                            }
                                                            else if (currentTile->getState() == "Triple Word") {
                                                                wordMultiplier *= 3;
                                                            }
                                                        }
                                                        wordScore += letterValue;
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    wordScore *= wordMultiplier;
                                    moveScore += wordScore;
                                }

                                scores[activeRack] += moveScore;
                                scoreText.setString("Player: " + std::to_string(scores[0]) + " | AI: " + std::to_string(scores[1]));
                                board.confirmTemporaryTiles();
                                statusText.setString("Valid word(s)! +" + std::to_string(moveScore) + " points");
                                racks[activeRack]->fillRack(bag);
                                activeRack = 1;
                            }
                            else {
                                yesNoDialog.show(invalidWords);
                                statusText.setString("Invalid word(s). Add to dictionary?");
                            }

                            selectedRow = -1;
                            selectedCol = -1;
                            selectedRackIndex = -1;
                        }
                        else {
                            statusText.setString("No tiles placed yet!");
                        }
                    }
                    else if (cancelButton.isMouseOver(window)) {
                        Tile* tempTiles[225];
                        int tempCount = 0;
                        board.getTemporaryTiles(tempTiles, tempCount);

                        for (int i = 0; i < tempCount; i++) {
                            char letter = tempTiles[i]->getLetter();
                            int value = tempTiles[i]->getValue();
                            racks[activeRack]->addLetter(letter, value);
                        }

                        board.clearTemporaryTiles();
                        statusText.setString("Move canceled. Try again.");
                        selectedRow = -1;
                        selectedCol = -1;
                        selectedRackIndex = -1;
                    }
                    else if (mousePos.x < 600 && mousePos.y < 600) {
                        int row = mousePos.y / 40;
                        int col = mousePos.x / 40;

                        if (row >= 0 && row < 15 && col >= 0 && col < 15) {
                            Tile* tile = board.getTile(row, col);
                            if (!tile->getIsPlaced()) {
                                selectedRow = row;
                                selectedCol = col;
                                statusText.setString("Tile selected. Now select a piece from your rack.");
                            }
                            else {
                                statusText.setString("This tile is already occupied.");
                            }
                        }
                    }
                    else if (selectedRow != -1 && selectedCol != -1) {
                        for (int i = 0; i < 7; ++i) {
                            Piece* p = racks[activeRack]->getTileAt(i);
                            if (p) {
                                int rackY = activeRack * 80 + 200;
                                sf::FloatRect bounds = p->getGlobalBounds(625 + i * 50, rackY);
                                if (bounds.contains(static_cast<sf::Vector2f>(mousePos))) {
                                    selectedRackIndex = i;
                                    Tile* tile = board.getTile(selectedRow, selectedCol);
                                    tile->setLetter(p->getLetter());
                                    tile->setValue(p->getValue());
                                    tile->setIsPlaced(true);
                                    tile->setIsTemporary(true);
                                    racks[activeRack]->removeTileAt(selectedRackIndex);
                                    selectedRow = -1;
                                    selectedCol = -1;
                                    selectedRackIndex = -1;
                                    statusText.setString("Piece placed. Select another tile or click Place.");
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        
        if (activeRack == 1 && !waitingForAIWordApproval) {
            statusText.setString("AI is thinking...");
            window.clear(sf::Color::Black);
            board.draw(window);
            for (int i = 0; i < 2; i++) {
                racks[i]->drawRack(window, 625, i * 80 + 200);
                if (i == activeRack) {
                    sf::RectangleShape highlight;
                    highlight.setSize(sf::Vector2f(350, 40));
                    highlight.setPosition(625, i * 80 + 200);
                    highlight.setFillColor(sf::Color::Transparent);
                    highlight.setOutlineColor(sf::Color::Yellow);
                    highlight.setOutlineThickness(2);
                    window.draw(highlight);
                }
            }
            placeButton.draw(window);
            cancelButton.draw(window);
            window.draw(statusText);
            window.draw(scoreText);
            yesNoDialog.draw(window);
            window.display();

            sf::sleep(sf::milliseconds(1000));

            std::string attemptedWord;
            bool isRandomWord, needsApproval;
            if (ai.makeMove(&board, racks[1], attemptedWord, isRandomWord, needsApproval)) {
                if (needsApproval) {
                    aiAttemptedWord = attemptedWord;
                    yesNoDialog.show(aiAttemptedWord);
                    statusText.setString("AI created: " + aiAttemptedWord + ". Accept this word?");
                    waitingForAIWordApproval = true;
                }
                else {
                    Tile* tempTiles[225];
                    int tempCount = 0;
                    board.getTemporaryTiles(tempTiles, tempCount);

                    int moveScore = 0;
                    std::string words[50];
                    int wordCount = 0;
                    board.getAllWords(words, wordCount);

                    for (int w = 0; w < wordCount; w++) {
                        int wordScore = 0;
                        int wordMultiplier = 1;
                        for (int i = 0; i < 15; i++) {
                            for (int j = 0; j < 15; j++) {
                                if (board.getTile(i, j)->getLetter() == words[w][0]) {
                                    bool foundHorizontal = true;
                                    for (int k = 0; words[w][k] != '\0'; k++) {
                                        if (j + k >= 15 || board.getTile(i, j + k)->getLetter() != words[w][k]) {
                                            foundHorizontal = false;
                                            break;
                                        }
                                    }

                                    bool foundVertical = true;
                                    for (int k = 0; words[w][k] != '\0'; k++) {
                                        if (i + k >= 15 || board.getTile(i + k, j)->getLetter() != words[w][k]) {
                                            foundVertical = false;
                                            break;
                                        }
                                    }

                                    if (foundHorizontal) {
                                        for (int k = 0; words[w][k] != '\0'; k++) {
                                            Tile* currentTile = board.getTile(i, j + k);
                                            int letterValue = currentTile->getValue();
                                            if (currentTile->getIsTemporary()) {
                                                if (currentTile->getState() == "Double Letter") {
                                                    letterValue *= 2;
                                                }
                                                else if (currentTile->getState() == "Triple Letter") {
                                                    letterValue *= 3;
                                                }
                                                if (currentTile->getState() == "Double Word") {
                                                    wordMultiplier *= 2;
                                                }
                                                else if (currentTile->getState() == "Triple Word") {
                                                    wordMultiplier *= 3;
                                                }
                                            }
                                            wordScore += letterValue;
                                        }
                                        break;
                                    }
                                    else if (foundVertical) {
                                        for (int k = 0; words[w][k] != '\0'; k++) {
                                            Tile* currentTile = board.getTile(i + k, j);
                                            int letterValue = currentTile->getValue();
                                            if (currentTile->getIsTemporary()) {
                                                if (currentTile->getState() == "Double Letter") {
                                                    letterValue *= 2;
                                                }
                                                else if (currentTile->getState() == "Triple Letter") {
                                                    letterValue *= 3;
                                                }
                                                if (currentTile->getState() == "Double Word") {
                                                    wordMultiplier *= 2;
                                                }
                                                else if (currentTile->getState() == "Triple Word") {
                                                    wordMultiplier *= 3;
                                                }
                                            }
                                            wordScore += letterValue;
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                        wordScore *= wordMultiplier;
                        moveScore += wordScore;
                    }
                    scores[activeRack] += moveScore;
                    board.confirmTemporaryTiles();
                    statusText.setString("AI played! +" + std::to_string(moveScore) + " points");
                    scoreText.setString("Player: " + std::to_string(scores[0]) + " | AI: " + std::to_string(scores[1]));
                    racks[activeRack]->fillRack(bag);
                    activeRack = 0;
                }
            }
            else {
                statusText.setString("AI passes turn.");
                activeRack = 0;
            }
        }

        if (waitingForAIWordApproval && event.type == sf::Event::MouseButtonPressed) {
            if (yesNoDialog.isYesClicked(window)) {
                dictionary.addWordToDictionary(aiAttemptedWord);

                Tile* tempTiles[225];
                int tempCount = 0;
                board.getTemporaryTiles(tempTiles, tempCount);

                int moveScore = 0;
                std::string words[50];
                int wordCount = 0;
                board.getAllWords(words, wordCount);

                for (int w = 0; w < wordCount; w++) {
                    int wordScore = 0;
                    int wordMultiplier = 1;
                    for (int i = 0; i < 15; i++) {
                        for (int j = 0; j < 15; j++) {
                            if (board.getTile(i, j)->getLetter() == words[w][0]) {
                                bool foundHorizontal = true;
                                for (int k = 0; words[w][k] != '\0'; k++) {
                                    if (j + k >= 15 || board.getTile(i, j + k)->getLetter() != words[w][k]) {
                                        foundHorizontal = false;
                                        break;
                                    }
                                }

                                bool foundVertical = true;
                                for (int k = 0; words[w][k] != '\0'; k++) {
                                    if (i + k >= 15 || board.getTile(i + k, j)->getLetter() != words[w][k]) {
                                        foundVertical = false;
                                        break;
                                    }
                                }

                                if (foundHorizontal) {
                                    for (int k = 0; words[w][k] != '\0'; k++) {
                                        Tile* currentTile = board.getTile(i, j + k);
                                        int letterValue = currentTile->getValue();
                                        if (currentTile->getIsTemporary()) {
                                            if (currentTile->getState() == "Double Letter") {
                                                letterValue *= 2;
                                            }
                                            else if (currentTile->getState() == "Triple Letter") {
                                                letterValue *= 3;
                                            }
                                            if (currentTile->getState() == "Double Word") {
                                                wordMultiplier *= 2;
                                            }
                                            else if (currentTile->getState() == "Triple Word") {
                                                wordMultiplier *= 3;
                                            }
                                        }
                                        wordScore += letterValue;
                                    }
                                    break;
                                }
                                else if (foundVertical) {
                                    for (int k = 0; words[w][k] != '\0'; k++) {
                                        Tile* currentTile = board.getTile(i + k, j);
                                        int letterValue = currentTile->getValue();
                                        if (currentTile->getIsTemporary()) {
                                            if (currentTile->getState() == "Double Letter") {
                                                letterValue *= 2;
                                            }
                                            else if (currentTile->getState() == "Triple Letter") {
                                                letterValue *= 3;
                                            }
                                            if (currentTile->getState() == "Double Word") {
                                                wordMultiplier *= 2;
                                            }
                                            else if (currentTile->getState() == "Triple Word") {
                                                wordMultiplier *= 3;
                                            }
                                        }
                                        wordScore += letterValue;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    wordScore *= wordMultiplier;
                    moveScore += wordScore;
                }
                scores[activeRack] += moveScore;
                board.confirmTemporaryTiles();
                statusText.setString("AI word accepted! +" + std::to_string(moveScore) + " points");
                scoreText.setString("Player: " + std::to_string(scores[0]) + " | AI: " + std::to_string(scores[1]));
                racks[activeRack]->fillRack(bag);
                activeRack = 0;
                yesNoDialog.hide();
                waitingForAIWordApproval = false;
            }
            else if (yesNoDialog.isNoClicked(window)) {
                Tile* tempTiles[225];
                int tempCount = 0;
                board.getTemporaryTiles(tempTiles, tempCount);

                for (int i = 0; i < tempCount; i++) {
                    char letter = tempTiles[i]->getLetter();
                    int value = tempTiles[i]->getValue();
                    racks[activeRack]->addLetter(letter, value);
                }

                board.clearTemporaryTiles();
                statusText.setString("AI word rejected. AI passes turn.");
                activeRack = 0;
                yesNoDialog.hide();
                waitingForAIWordApproval = false;
            }
        }

        if (placeButton.isMouseOver(window)) {
            placeButton.setBackColor(sf::Color(150, 150, 150));
        }
        else {
            placeButton.setBackColor(sf::Color(100, 100, 100));
        }

        if (cancelButton.isMouseOver(window)) {
            cancelButton.setBackColor(sf::Color(150, 150, 150));
        }
        else {
            cancelButton.setBackColor(sf::Color(100, 100, 100));
        }

        yesNoDialog.update(window);

        window.clear(sf::Color::Black);
        board.draw(window);
        for (int i = 0; i < 2; i++) {
            racks[i]->drawRack(window, 625, i * 80 + 200);
            if (i == activeRack) {
                sf::RectangleShape highlight;
                highlight.setSize(sf::Vector2f(350, 40));
                highlight.setPosition(625, i * 80 + 200);
                highlight.setFillColor(sf::Color::Transparent);
                highlight.setOutlineColor(sf::Color::Yellow);
                highlight.setOutlineThickness(2);
                window.draw(highlight);
            }
        }
        if (selectedRow != -1 && selectedCol != -1) {
            sf::RectangleShape highlight;
            highlight.setSize(sf::Vector2f(40, 40));
            highlight.setPosition(selectedCol * 40, selectedRow * 40);
            highlight.setFillColor(sf::Color::Transparent);
            highlight.setOutlineColor(sf::Color::Yellow);
            highlight.setOutlineThickness(2);
            window.draw(highlight);
        }

        placeButton.draw(window);
        cancelButton.draw(window);
        window.draw(statusText);
        window.draw(scoreText);
        yesNoDialog.draw(window);

        window.display();
    }

    for (int i = 0; i < 2; i++) {
        delete racks[i];
    }

    return 0;
}
