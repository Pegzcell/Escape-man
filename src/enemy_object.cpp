/******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "enemy_object.h"

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <vector>

EnemyObject::EnemyObject()
    : GameObject(), Radius(12.5f), Stuck(true) {}

EnemyObject::EnemyObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, Texture2D sprite, glm::vec3 color, glm::vec2 offset)
    : GameObject(pos, size, sprite, color, offset, velocity), Stuck(true) {}

glm::vec2 EnemyObject::Move(float dt, unsigned int window_width)
{
    // if not stuck to player board
    if (1)
    {
        // move the enemy
        this->Position += this->Velocity * dt;
    }
    return this->Position;
}

// resets the enemy to initial Stuck Position (if enemy is outside window bounds)
void EnemyObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
}
