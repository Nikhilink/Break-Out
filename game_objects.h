#pragma once

#include "raylib.h"

class Paddle
{
    public:
        Rectangle paddle;
        Rectangle position;

};

class Ball
{
    public:
        Rectangle ball;
        Rectangle position;
};

enum GameStates
{
    Ready, Play, Pause, Lose
};