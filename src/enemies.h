/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef ENEMIES_H
#define ENEMIES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Enemies
{
public:
    unsigned int Count, N, M;
    std::vector<std::vector<bool>> Enemy_points;

    // constructor/destructor
    Enemies(unsigned int count, unsigned int n, unsigned int m);
    ~Enemies();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    // void ProcessInput(float dt);
    // void Update(float dt);
    // void Render();
};

#endif