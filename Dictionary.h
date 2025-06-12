#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>

class Dictionary {
private:
    std::string filename;
public:
    Dictionary(std::string filename);
    bool isValidWord(std::string word);
    bool addWordToDictionary(std::string word);
};

#endif // DICTIONARY_H