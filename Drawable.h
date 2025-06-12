#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <SFML/Graphics.hpp>

class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void draw(sf::RenderWindow& window) = 0;
};

#endif // DRAWABLE_H