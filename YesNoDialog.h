#ifndef YES_NO_DIALOG_H
#define YES_NO_DIALOG_H

#include "Button.h"
#include <string>

class YesNoDialog {
private:
    Button yesButton;
    Button noButton;
    sf::Text promptText;
    bool visible;
    std::string invalidWords;
public:
    YesNoDialog(sf::Font& font);
    void show(std::string words);
    void hide();
    bool isVisible();
    std::string getInvalidWords();
    bool isYesClicked(sf::RenderWindow& window);
    bool isNoClicked(sf::RenderWindow& window);
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
};

#endif // YES_NO_DIALOG_H