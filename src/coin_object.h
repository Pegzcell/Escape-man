/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef COINOBJECT_H
#define COINOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"

#include <vector>

// CoinObject holds the state of the Coin object inheriting
// relevant state data from GameObject. Contains some extra
// functionality specific to Breakout's coin object that
// were too specific for within GameObject alone.
class CoinObject : public GameObject
{
public:
    // coin state
    float Radius;
    bool Stuck;
    
    // constructor(s)
    CoinObject();
    CoinObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, Texture2D sprite, glm::vec3 color, glm::vec2 offset);
    // moves the coin, keeping it constrained within the window bounds (except bottom edge); returns new position
    glm::vec2 Move(float dt, unsigned int window_width);
    // resets the coin to original state with given position and velocity
    void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif