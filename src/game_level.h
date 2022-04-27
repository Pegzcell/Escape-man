/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "enemy_object.h"
#include "coin_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"
#include "walls.h"

const glm::vec2 BOUNDARY_TILES(40.0f, 20.0f); // no. of tiles in x and y direction
const float TILE_WALL_RATIO = 50.0f;
const int WALL_CONSTANT = 80;

/// GameLevel holds all Tiles as part of a Breakout level and
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel
{
public:
    unsigned int LevelWidth, LevelHeight, X_tiles, Y_tiles, Enemy_count, Coin_count;
    float tile_dim_x, tile_dim_y, x_wall_width, y_wall_width;
    glm::vec2 Player_pos, Door_pos, Offset;
    glm::vec2 doorSize, doorPos, playerSize, playerPos;
    glm::vec2 levelLightSize;
    // level state
    std::vector<GameObject> Boundary;
    GameObject Door;
    std::vector<EnemyObject> Enemy;
    std::vector<CoinObject> Coin;
    std::vector<std::vector<cell>> Maze;
    int levelCoins;
    // constructor
    GameLevel(unsigned int levelWidth, unsigned int levelHeight, unsigned int x_tiles, unsigned int y_tiles, unsigned int enemy_count, unsigned int coin_count, glm::vec2 offset);
    // loads level from file
    // render level
    void Load();
    void Draw(SpriteRenderer &renderer);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();

private:
    // initialize level from tile data
    //void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
    void init(std::vector<std::vector<bool>> EnemyPoints, std::vector<std::vector<bool>> CoinPoints);
};

#endif