/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include <time.h>
#include <unistd.h>
#include <iostream>

// Game-related State data
SpriteRenderer *Renderer;
GameObject *Player;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
    delete Renderer;
    delete Player;
}

void Game::Init()
{
    srand(time(0));
    // load shaders
    ResourceManager::LoadShader("../src/shaders/sprite.vs", "../src/shaders/sprite.frag", nullptr, "sprite");
    // ResourceManager::LoadShader("../src/shaders/light.vs", "../src/shaders/light.frag", nullptr, "light");
    // configure shaders
    Left = 0.0f;
    Projection = glm::ortho(Left, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", Projection);
    // set render-specific controls
    Shader shader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(shader);
    // load textures
    ResourceManager::LoadTexture("../src/textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("../src/textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("../src/textures/block.png", false, "block");
    ResourceManager::LoadTexture("../src/textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("../src/textures/awesomeface.png", true, "player");
    ResourceManager::LoadTexture("../src/textures/wall.jpg", false, "wall");
    ResourceManager::LoadTexture("../src/textures/coin.png", true, "coin");
    ResourceManager::LoadTexture("../src/textures/try.png", true, "tunnel");
    ResourceManager::LoadTexture("../src/textures/gameover.png", true, "gameover");
    ResourceManager::LoadTexture("../src/textures/win.png", true, "victory");
    // load levels
    glm::vec2 offset(Width, 0.0f);
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> GameLevel one; one.Load("../src/levels/one.lvl", Width, Height / 2);
    GameLevel one(Width, Height, 3, 3, 1, 2, offset - offset);
    GameLevel two(Width, Height, 5, 5, 2, 5, offset);
    GameLevel three(Width, Height, 7, 7, 4, 10, offset + offset);
    one.Load();
    // two.Load();
    // three.Load();
    // GameLevel two; two.Load("levels/two.lvl", Width, Height / 2);
    // GameLevel three; three.Load("levels/three.lvl", Width, Height / 2);
    // GameLevel four; four.Load("levels/four.lvl", Width, Height / 2);
    Levels.push_back(one);
    Levels.push_back(two);
    Levels.push_back(three);
    // Levels.push_back(two);
    // Levels.push_back(three);
    // Levels.push_back(four);
    Level = 0;
    // configure game objects
    Levels[Level].playerSize = glm::vec2(Levels[Level].doorSize.y * 0.5, Levels[Level].doorSize.y * 0.5);
    Levels[Level].playerPos = glm::vec2(Levels[Level].y_wall_width, Levels[Level].LevelHeight / 2 - Levels[Level].playerSize.y / 2);

    Player = new GameObject(Levels[Level].playerPos, Levels[Level].playerSize, ResourceManager::GetTexture("player"));
}

void Game::NextLevel()
{
    std::cout << Player->Position.x << ", " << Player->Position.y << "\t" << Player->Offset.x << ", " << Player->Position.y << std::endl;
    if (Level == 2)
    {
        State = GAME_WIN;
        return;
    }
    Level++;
    Levels[Level - 1].playerPos = Player->Position;
    Levels[Level].Load();
    Player->Offset = Levels[Level].Offset;
    Player->Position -= (Levels[Level].Offset - Levels[Level - 1].Offset);
    Levels[Level].playerSize = glm::vec2(Levels[Level].doorSize.y * 0.5, Levels[Level].doorSize.y * 0.5);
    Levels[Level].playerPos = glm::vec2(Levels[Level].y_wall_width, Levels[Level].LevelHeight / 2 - Levels[Level].playerSize.y / 2);
    State = GAME_LEVELLING;
    // Player->Position = PLAYER_INITIAL;

    std::cout << Player->Position.x << ", " << Player->Position.y << "\t" << Player->Offset.x << ", " << Player->Position.y << std::endl;
}
bool CheckCollisionQQ(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
                      two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
                      two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}
bool CheckCollisionCQ(GameObject &one, GameObject &two) // AABB - Circle collision
{
    // get center point circle first
    glm::vec2 center(one.Position + one.Radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(
        two.Position.x + aabb_half_extents.x,
        two.Position.y + aabb_half_extents.y);
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;
    return glm::length(difference) < one.Radius;
}

bool CheckCollisionCC(GameObject &one, GameObject &two) // Circle - Circle collision
{
    // get center point of circles
    glm::vec2 center1(one.Position + one.Radius);
    glm::vec2 center2(two.Position + two.Radius);
    // calculate AABB info (center, half-extents)
    // glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    // glm::vec2 aabb_center(
    //     two.Position.x + aabb_half_extents.x,
    //     two.Position.y + aabb_half_extents.y
    // );
    // // get difference vector between both centers
    // glm::vec2 difference = center - aabb_center;
    // glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // // add clamped value to AABB_center and we get the value of box closest to circle
    // glm::vec2 closest = aabb_center + clamped;
    // // retrieve vector between center circle and closest point AABB and check if length <= radius
    glm::vec2 difference = center1 - center2;
    return glm::length(difference) <= one.Radius + two.Radius;
}
void Game::DoCollisions()
{
    //Enemy-Wall
    int i, j;
    for (GameObject &e : Levels[Level].Enemy)
    {
        glm::vec2 centre(e.Position + e.Radius);
        j = centre.x / (Levels[Level].y_wall_width + Levels[Level].tile_dim_x);
        i = centre.y / (Levels[Level].x_wall_width + Levels[Level].tile_dim_y);
        // std::cout << "(" << i << ", " << j << ")" << std::endl;

        if (e.Velocity.x < 0)
        {
            if ((j == 0 && e.Position.x <= Levels[Level].y_wall_width) || (j != 0 && Levels[Level].Maze[i][j - 1].right && CheckCollisionQQ(e, Levels[Level].Boundary[Levels[Level].Maze[i][j - 1].push_index_right])))
                e.Velocity.x = -e.Velocity.x;
        }
        else if (e.Velocity.x > 0)
        {
            if (((j == Levels[Level].Y_tiles - 1) && e.Position.x >= Width - Levels[Level].y_wall_width - e.Size.x) || (Levels[Level].Maze[i][j].right && CheckCollisionQQ(e, Levels[Level].Boundary[Levels[Level].Maze[i][j].push_index_right])))
                e.Velocity.x = -e.Velocity.x;
        }
        if (e.Velocity.y < 0)
        {
            if ((i == 0 && e.Position.y <= Levels[Level].x_wall_width) || (i != 0 && Levels[Level].Maze[i - 1][j].down && CheckCollisionQQ(e, Levels[Level].Boundary[Levels[Level].Maze[i - 1][j].push_index_down])))
                e.Velocity.y = -e.Velocity.y;
        }
        else if (e.Velocity.y > 0)
        {
            if (((i == Levels[Level].X_tiles - 1) && e.Position.y >= Height - Levels[Level].x_wall_width - e.Size.y) || (Levels[Level].Maze[i][j].down && CheckCollisionQQ(e, Levels[Level].Boundary[Levels[Level].Maze[i][j].push_index_down])))
                e.Velocity.y = -e.Velocity.y;
        }
    }

    //Coin-Player
    for (GameObject &coin : Levels[Level].Coin)
    {
        if (!coin.Destroyed)
            if (CheckCollisionCC(*Player, coin))
            {
                coin.Destroyed = true;
            }
    }

    //Enemy-Player
    for (GameObject &e : Levels[Level].Enemy)
    {

        if (CheckCollisionCC(*Player, e))
        {
            Player->Destroyed = true;
            State = GAME_LOST;
            NextLevel();
        }
    }
    //Player-Door
    if (CheckCollisionCQ(*Player, Levels[Level].Door))
    {
        NextLevel();
    }
    //Enemy-Enemy
    // for (GameObject &e : Levels[Level].Enemy)
    // {
    //     for (GameObject &f : Levels[Level].Enemy)
    //         if (e.Position != f.Position && CheckCollisionCC(f, e))
    //         {
    //             e.Velocity = -e.Velocity;
    //             e.Position += e.Velocity;
    //             f.Velocity = -f.Velocity;
    //             f.Position += f.Velocity;
    //         }
    // }
}

void Game::Update(float dt)
{
    if (State == GAME_ACTIVE)
    {
        for (CoinObject &coin : Levels[Level].Coin)
        {
            coin.Move(dt, Width);
        }
        for (EnemyObject &enemy : Levels[Level].Enemy)
        {
            enemy.Move(dt, Width);
        }
        DoCollisions();
    }
}

void Game::ProcessInput(float dt)
{
    if (State == GAME_ACTIVE)
    {
        int i, j;
        glm::vec2 centre(Player->Position + Player->Radius);
        j = (centre.x) / (Levels[Level].y_wall_width + Levels[Level].tile_dim_x);
        i = (centre.y) / (Levels[Level].x_wall_width + Levels[Level].tile_dim_y);
        // std::cout << "(" << i << ", " << j << ")" << std::endl;

        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (Keys[GLFW_KEY_A])
        {
            if ((j == 0 && Player->Position.x <= Levels[Level].y_wall_width) || (j != 0 && Levels[Level].Maze[i][j - 1].right && CheckCollisionQQ(*Player, Levels[Level].Boundary[Levels[Level].Maze[i][j - 1].push_index_right])))
                ;
            else
                Player->Position.x -= velocity;
        }
        else if (Keys[GLFW_KEY_D])
        {
            if (((j == Levels[Level].Y_tiles - 1) && Player->Position.x >= Width - Levels[Level].y_wall_width - Player->Size.x) || (Levels[Level].Maze[i][j].right && CheckCollisionQQ(*Player, Levels[Level].Boundary[Levels[Level].Maze[i][j].push_index_right])))
                ;
            else
                Player->Position.x += velocity;
        }
        if (Keys[GLFW_KEY_W])
        {
            if ((i == 0 && Player->Position.y <= Levels[Level].x_wall_width) ||
                (i != 0 && Levels[Level].Maze[i - 1][j].down && CheckCollisionQQ(*Player, Levels[Level].Boundary[Levels[Level].Maze[i - 1][j].push_index_down])))
                // || (i !=0 && j != 0 && Levels[Level].Maze[i-1][j - 1].right) && CheckCollisionQQ(*Player, Levels[Level].Boundary[Levels[Level].Maze[i-1][j - 1].push_index_right]))
                // (Levels[Level].Maze[i][j].right) && CheckCollisionQQ(*Player, Levels[Level].Boundary[Levels[Level].Maze[i][j].push_index_right])
                ;
            else
                Player->Position.y -= velocity;
        }
        else if (Keys[GLFW_KEY_S])
        {
            if (((i == Levels[Level].X_tiles - 1) && Player->Position.y >= Height - Levels[Level].x_wall_width - Player->Size.y) || (Levels[Level].Maze[i][j].down && CheckCollisionQQ(*Player, Levels[Level].Boundary[Levels[Level].Maze[i][j].push_index_down])))
                ;
            else
                Player->Position.y += velocity;
        }
    }
}

void Game::Render(float dt)
{
    //camera
    if (Level && (State == GAME_LEVELLING))
    {
        //std::cout << Player->Position.x << ", " << Player->Position.y << "\t" << Player->Offset.x << ", " << Player->Position.y << std::endl;
        if (Left >= Levels[Level].Offset.x)
        {
            State = GAME_ACTIVE;
        }
        Left += (dt * CAMERA_SPEED) > Levels[Level].Offset.x - Left ? Levels[Level].Offset.x - Left : (dt * CAMERA_SPEED);
        Projection = glm::ortho(Left, Left + (float)Width, (float)Height, 0.0f, -1.0f, 1.0f);
        ResourceManager::GetShader("sprite").SetMatrix4("projection", Projection);
        float playerSizeDeterioraton = (dt * CAMERA_SPEED) * ((Levels[Level - 1].playerSize.y - Levels[Level].playerSize.y) / Width);
        float playerSpeed = (CAMERA_SPEED) * ((Levels[Level - 1].doorSize.x / 2 + (Levels[Level - 1].LevelWidth - Levels[Level - 1].playerPos.x)) / Width);
        float playerSpeed_y = (CAMERA_SPEED) * ((Levels[Level].playerPos.y - Levels[Level - 1].playerPos.y) / Width);
        Player->Size -= glm::vec2(playerSizeDeterioraton, playerSizeDeterioraton);
        Player->Radius = (Player->Size.x + Player->Size.y) / 4;
        Player->Position += glm::vec2((dt * playerSpeed), (dt * playerSpeed_y));
    }
    // draw background
    Texture2D bg_texture = ResourceManager::GetTexture("background");

    Renderer->DrawSprite(bg_texture, Levels[Level].Offset, glm::vec2(Width, Height), 0.0f);
    // draw level
    Levels[Level].Draw(*Renderer);
    // draw player
    if (Level)
    {
        Renderer->DrawSprite(bg_texture, Levels[Level - 1].Offset, glm::vec2(Width, Height), 0.0f);
        Levels[Level - 1].Draw(*Renderer);
    }
    if (!Player->Destroyed)
        Player->Draw(*Renderer);
    if (State == GAME_WIN)
    {
        Texture2D go_texture = ResourceManager::GetTexture("victory");
        Renderer->DrawSprite(go_texture, Levels[Level].Offset, glm::vec2(Width, Height));
    }
    else if (State == GAME_LOST)
    {
        Texture2D go_texture = ResourceManager::GetTexture("gameover");
        Renderer->DrawSprite(go_texture, Levels[Level].Offset, glm::vec2(Width, Height));
    }
}