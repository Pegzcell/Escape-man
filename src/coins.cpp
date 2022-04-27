/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "coins.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <vector>

using namespace std;

Coins::Coins(unsigned int count, unsigned int n, unsigned int m)
    : Count(count), N(n), M(m)
{
}

Coins::~Coins()
{
    // delete Renderer;
    // delete Player;
}

void Coins::Init()
{
    int a = -1, b = -1;
    a = (N / 2) * M;
    if (N % 2 == 0)
    {
        b = (N / 2 - 1) * M;
    }
    bool array[M * N] = {false};
    int c = 0, t;
    while (c != Count)
    {
        t = rand() % (N * M);
        if (t != a && t != b && !array[t])
        {
            array[t] = true;
            c++;
        }
    }
    t = 0;

    for (int i = 0; i < N; i++)
    {
        std::vector<bool> obj_line;
        Coin_points.push_back(obj_line);
        for (int j = 0; j < M; j++)
        {
            Coin_points[i].push_back(array[t++]);
        }
    }

    // for (int i = 0; i < N; i++)
    // {
    //     for (int j = 0; j < M; j++)
    //     {
    //         cout << Coin_points[i][j] << "\t";
    //     }
    //     cout << endl;
    // }
};