/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef WALLS_H
#define WALLS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>


typedef struct cell
{
    bool visited, right, down;
    int push_index_right, push_index_down;
} cell;


// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Walls
{
public:
    unsigned int Width, Height;
    unsigned int N, M;
    std::vector<std::vector<cell>> Maze;

    // constructor/destructor
    Walls(unsigned int width, unsigned int height, unsigned int n, unsigned int m);
    ~Walls();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    // void ProcessInput(float dt);
    // void Update(float dt);
    // void Render();

private:
    void recursor(int i, int j);
};

#endif