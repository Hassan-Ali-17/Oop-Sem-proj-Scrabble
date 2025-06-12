#include "GameEntity.h"

GameEntity::GameEntity(float x, float y) : position(x, y) {}

void GameEntity::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

sf::Vector2f GameEntity::getPosition() const {
    return position;
}