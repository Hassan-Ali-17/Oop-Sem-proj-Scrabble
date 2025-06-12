#include "Dictionary.h"
#include <fstream>
#include <iostream>
#include <cctype>

Dictionary::Dictionary(std::string filename) {
    this->filename = filename;
}

bool Dictionary::isValidWord(std::string word) {
    for (int i = 0; i < word.length(); i++) {
        word[i] = std::tolower(word[i]);
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open dictionary file!" << std::endl;
        return false;
    }

    std::string dictWord;
    while (std::getline(file, dictWord)) {
        for (int i = 0; i < dictWord.length(); i++) {
            dictWord[i] = std::tolower(dictWord[i]);
        }
        while (!dictWord.empty() && std::isspace(dictWord.back())) {
            dictWord.pop_back();
        }

        if (word == dictWord) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

bool Dictionary::addWordToDictionary(std::string word) {
    for (int i = 0; i < word.length(); i++) {
        word[i] = std::tolower(word[i]);
    }

    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cout << "Failed to open dictionary file for writing!" << std::endl;
        return false;
    }

    file << std::endl << word;
    file.close();
    return true;
}