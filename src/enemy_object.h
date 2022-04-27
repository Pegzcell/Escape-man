/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef ENEMYOBJECT_H
#define ENEMYOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"

#include <vector>

// EnemyObject holds the state of the Enemy object inheriting
// relevant state data from GameObject. Contains some extra
// functionality specific to Breakout's enemy object that
// were too specific for within GameObject alone.
class EnemyObject : public GameObject
{
public:
    // enemy state
    float Radius;
    bool Stuck;
    
    // constructor(s)
    EnemyObject();
    EnemyObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, Texture2D sprite, glm::vec3 color, glm::vec2 offset);
    // moves the enemy, keeping it constrained within the window bounds (except bottom edge); returns new position
    glm::vec2 Move(float dt, unsigned int window_width);
    // resets the enemy to original state with given position and velocity
    void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif