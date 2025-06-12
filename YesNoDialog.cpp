#include "YesNoDialog.h"

YesNoDialog::YesNoDialog(sf::Font& font) :
    yesButton(700, 300, 80, 40, font, "Yes"),
    noButton(800, 300, 80, 40, font, "No") {

    promptText.setFont(font);
    promptText.setCharacterSize(16);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(625, 250);
    visible = false;
    invalidWords = "";
}

void YesNoDialog::show(std::string words) {
    visible = true;
    invalidWords = words;
    promptText.setString("Add invalid word(s) to dictionary?\n" + words);
}

void YesNoDialog::hide() {
    visible = false;
}

bool YesNoDialog::isVisible() {
    return visible;
}

std::string YesNoDialog::getInvalidWords() {
    return invalidWords;
}

bool YesNoDialog::isYesClicked(sf::RenderWindow& window) {
    return visible && yesButton.isMouseOver(window);
}

bool YesNoDialog::isNoClicked(sf::RenderWindow& window) {
    return visible && noButton.isMouseOver(window);
}

void YesNoDialog::update(sf::RenderWindow& window) {
    if (visible) {
        if (yesButton.isMouseOver(window)) {
            yesButton.setBackColor(sf::Color(150, 150, 150));
        }
        else {
            yesButton.setBackColor(sf::Color(100, 100, 100));
        }

        if (noButton.isMouseOver(window)) {
            noButton.setBackColor(sf::Color(150, 150, 150));
        }
        else {
            noButton.setBackColor(sf::Color(100, 100, 100));
        }
    }
}

void YesNoDialog::draw(sf::RenderWindow& window) {
    if (visible) {
        window.draw(promptText);
        yesButton.draw(window);
        noButton.draw(window);
    }
}