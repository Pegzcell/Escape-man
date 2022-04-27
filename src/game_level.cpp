/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game_level.h"
#include <stdio.h>
#include "walls.h"
#include "enemy_object.h"
#include "enemies.h"
#include "coin_object.h"
#include "coins.h"
#include "iostream"
#include "unistd.h"
#include <fstream>
#include <sstream>

GameLevel::GameLevel(unsigned int levelWidth, unsigned int levelHeight, unsigned int x_tiles, unsigned int y_tiles, unsigned int enemy_count, unsigned int coin_count, glm::vec2 offset)
    : LevelWidth(levelWidth), LevelHeight(levelHeight), X_tiles(x_tiles), Y_tiles(y_tiles), Enemy_count(enemy_count), Coin_count(coin_count), Offset(offset), levelCoins(coin_count)
{
}

void GameLevel::Load()
{
    // clear old data
    this->Boundary.clear();
    // load from file
    Walls walls(LevelWidth, LevelHeight, Y_tiles, X_tiles);
    walls.Init();

    Enemies enemies(Enemy_count, Y_tiles, X_tiles);
    enemies.Init();

    Coins coins(Coin_count, Y_tiles, X_tiles);
    coins.Init();
    /*
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            //this->init(tileData, levelWidth, levelHeight);
            this->init(levelWidth, levelHeight);
    }
    */
    Maze = walls.Maze;
    this->init(enemies.Enemy_points, coins.Coin_points);
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    // WALLS
    for (GameObject &tile : this->Boundary)
        tile.Draw(renderer);

    // DOOR
    this->Door.Draw(renderer);

    // COINS
    for (CoinObject &coin : this->Coin)
        if (!coin.Destroyed)
            coin.Draw(renderer);

    // ENEMIES
    for (EnemyObject &enemyy : this->Enemy)
    {

        enemyy.Draw(renderer);
    }
}

bool GameLevel::IsCompleted()
{
    // for (GameObject &tile : this->Bricks)
    //     if (!tile.IsSolid && !tile.Destroyed)
    //         return false;
    return false;
}

bool decide_wall(int factor)
{
    return rand() % 100 < factor;
}

void GameLevel::init(std::vector<std::vector<bool>> EnemyPoints, std::vector<std::vector<bool>> CoinPoints)
{
    // WALLS----------------------------------------------------------------------------------------
    int pushIndex = 0;
    y_wall_width = LevelWidth / (1 + (TILE_WALL_RATIO + 1) * X_tiles);
    x_wall_width = LevelHeight / (1 + (TILE_WALL_RATIO + 1) * Y_tiles);
    tile_dim_x = y_wall_width * TILE_WALL_RATIO;
    tile_dim_y = x_wall_width * TILE_WALL_RATIO;

    levelLightSize = glm::vec2(tile_dim_x * 10, tile_dim_y * 10);
    //left boundary
    for (int i = 0; i < Y_tiles; i++)
    {
        glm::vec2 pos(0, (tile_dim_y + x_wall_width) * i);
        GameObject obj(pos, glm::vec2(y_wall_width, tile_dim_y + x_wall_width), ResourceManager::GetTexture("wall"), glm::vec3(0.8f, 0.8f, 0.7f), Offset);
        this->Boundary.push_back(obj);
        pushIndex++;

        pos = glm::vec2(LevelHeight - y_wall_width, (tile_dim_y + x_wall_width) * i);
        obj = GameObject(pos, glm::vec2(y_wall_width, tile_dim_y + x_wall_width), ResourceManager::GetTexture("wall"), glm::vec3(0.8f, 0.8f, 0.7f), Offset);
        this->Boundary.push_back(obj);
        pushIndex++;
    }
    //top boundary
    for (int i = 0; i < X_tiles; i++)
    {
        glm::vec2 pos((tile_dim_x + y_wall_width) * i, 0);
        GameObject obj(pos, glm::vec2(tile_dim_x + y_wall_width, x_wall_width), ResourceManager::GetTexture("wall"), glm::vec3(0.8f, 0.8f, 0.7f), Offset);
        this->Boundary.push_back(obj);
        pushIndex++;

        pos = glm::vec2((tile_dim_x + y_wall_width) * i, LevelHeight - x_wall_width);
        obj = GameObject(pos, glm::vec2(tile_dim_x + y_wall_width, x_wall_width), ResourceManager::GetTexture("wall"), glm::vec3(0.8f, 0.8f, 0.7f), Offset);
        this->Boundary.push_back(obj);
        pushIndex++;
    }
    //right and bottom
    for (int i = 0; i < Y_tiles; i++)
    {
        for (int j = 0; j < X_tiles; j++)
        {
            //right
            if (Maze[i][j].right)
            {
                if (decide_wall(WALL_CONSTANT))
                {
                    glm::vec2 pos((tile_dim_x + y_wall_width) * (j + 1), (tile_dim_y + x_wall_width) * i);
                    GameObject obj(pos, glm::vec2(y_wall_width, tile_dim_y + x_wall_width), ResourceManager::GetTexture("wall"), glm::vec3(0.8f, 0.8f, 0.7f), Offset);
                    this->Boundary.push_back(obj);
                    Maze[i][j].push_index_right = pushIndex;
                    pushIndex++;
                }
                else
                {
                    Maze[i][j].right = false;
                }
            }
            //bottom
            if (Maze[i][j].down)
            {
                if (decide_wall(WALL_CONSTANT))
                {
                    glm::vec2 pos((tile_dim_x + y_wall_width) * j, (tile_dim_y + x_wall_width) * (i + 1));
                    GameObject obj(pos, glm::vec2(tile_dim_x + y_wall_width, x_wall_width), ResourceManager::GetTexture("wall"), glm::vec3(0.8f, 0.8f, 0.7f), Offset);
                    this->Boundary.push_back(obj);
                    Maze[i][j].push_index_down = pushIndex;
                    pushIndex++;
                }
                else
                {
                    Maze[i][j].down = false;
                }
            }
            //end-corner
            glm::vec2 pos(LevelWidth - y_wall_width, LevelHeight - x_wall_width);
            GameObject obj(pos, glm::vec2(x_wall_width, y_wall_width), ResourceManager::GetTexture("wall"), glm::vec3(0.8f, 0.8f, 0.7f), Offset);
            this->Boundary.push_back(obj);
            pushIndex++;
        }
    }

    // DOOR----------------------------------------------------------------------------------------
    doorSize = glm::vec2(tile_dim_x / 4 + y_wall_width * 2, tile_dim_y * 0.7);
    doorPos = glm::vec2(LevelWidth - doorSize.x / 2, LevelHeight / 2 - doorSize.y / 2);
    this->Door = GameObject(doorPos, doorSize, ResourceManager::GetTexture("tunnel"), glm::vec3(1.0f, 0.8f, 0.7f), Offset);
    // ENEMIES----------------------------------------------------------------------------------------
    for (int i = 0; i < Y_tiles; i++)
    {
        for (int j = 0; j < X_tiles; j++)
        {
            if (EnemyPoints[i][j])
            {
                glm::vec2 esize(tile_dim_x / 3, tile_dim_y / 3);
                glm::vec2 pos(y_wall_width + (tile_dim_x + y_wall_width) * j + tile_dim_x / 2 - esize.x / 2, x_wall_width + (tile_dim_y + x_wall_width) * i + tile_dim_y / 2 - esize.y / 2);
                EnemyObject obj(pos, esize, glm::vec2(50.0f*(rand()%2?1:-1), 60.0f*(rand()%2?1:-1)), ResourceManager::GetTexture("face"), glm::vec3(((rand()%50 + 1)/100.0f)+0.5f, 0.0f, 0.0f), Offset);
                obj.Stuck == false;
                // std::cout << obj.Stuck << "\t" << obj.Velocity.x << "\t" << obj.Velocity.y << std::endl;
                this->Enemy.push_back(obj);
            }
        }
    }

    // COINS-------------------------------------------------------------------------------------------
    for (int i = 0; i < Y_tiles; i++)
    {
        for (int j = 0; j < X_tiles; j++)
        {
            if (CoinPoints[i][j])
            {
                glm::vec2 esize(tile_dim_x / 4, tile_dim_y / 4);
                glm::vec2 pos((tile_dim_x + y_wall_width) * j + tile_dim_x / 2 - esize.x / 2, (tile_dim_y + x_wall_width) * i + tile_dim_y / 2 - esize.y / 2);
                CoinObject obj(pos, esize, glm::vec2(20.0f, 20.0f), ResourceManager::GetTexture("coin"), glm::vec3(1.0f), Offset);
                obj.Stuck == false;
                // std::cout << obj.Stuck << "\t" << obj.Velocity.x << "\t" << obj.Velocity.y << std::endl;
                this->Coin.push_back(obj);
            }
        }
    }
    //-------------
    /*
    this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
    // calculate dimensions
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / height;
    // initialize level tiles based on tileData
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            if (tileData[y][x] == 1) // solid
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                obj.IsSolid = true;
                this->Bricks.push_back(obj);
            }
            else if (tileData[y][x] > 1) // non-solid; now determine its color based on level data
            {
                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (tileData[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
            }
        }
    }
    */
}