#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <SFML/Graphics.hpp>

class GameEntity {
protected:
    sf::Vector2f position;
public:
    GameEntity(float x = 0, float y = 0);
    virtual ~GameEntity() = default;

    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
};

#endif // GAME_ENTITY_H