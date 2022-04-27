/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "walls.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <vector>

Walls::Walls(unsigned int width, unsigned int height, unsigned int n, unsigned int m)
    : Width(width), Height(height), N(n), M(m)
{
}

Walls::~Walls()
{
    // delete Renderer;
    // delete Player;
}
void Walls::recursor(int i, int j)
{
    if (i == 0 && j == 0 && Maze[i][j].visited)
    {
        return;
    }
    Maze[i][j].visited = true;
    std::vector<int> checker;
    int t;
    int temp;
    while (checker.size() != 4)
    {
        temp = rand() % 4;
        if (std::find(checker.begin(), checker.end(), temp) == checker.end())
        {
            t = temp;
            checker.push_back(t);
        }
        else
            continue;
        if (t == 0)
        {
            if (i > 0 && !Maze[i - 1][j].visited)
            {
                Maze[i - 1][j].down = false;
                recursor(i - 1, j);
            }
        }
        else if (t == 1)
        {
            if (j < M - 1 && !Maze[i][j + 1].visited)
            {
                Maze[i][j].right = false;
                recursor(i, j + 1);
            }
        }
        else if (t == 2)
        {
            if (i < N - 1 && !Maze[i + 1][j].visited)
            {
                Maze[i][j].down = false;
                recursor(i + 1, j);
            }
        }
        else if (t == 3)
        {
            if (j > 0 && !Maze[i][j - 1].visited)
            {
                Maze[i][j - 1].right = false;
                recursor(i, j - 1);
            }
        }
    }
}

void Walls::Init()
{
    srand(time(0));
    //initialize Maze with visited = 0, right = 1, down =1
    for (int i = 0; i < N; i++)
    {
        std::vector<cell> obj_line;
        Maze.push_back(obj_line);
        for (int j = 0; j < M; j++)
        {
            cell obj;
            obj.visited = 0;
            obj.right = 1;
            obj.down = 1;
            Maze[i].push_back(obj);
        }
    }
    recursor(0, 0);
}