/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef COINS_H
#define COINS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Coins
{
public:
    unsigned int Count, N, M;
    std::vector<std::vector<bool>> Coin_points;

    // constructor/destructor
    Coins(unsigned int count, unsigned int n, unsigned int m);
    ~Coins();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    // void ProcessInput(float dt);
    // void Update(float dt);
    // void Render();
};

#endif