/**
 * @file Robot.h
 * @author Giuseppe Sensolini (https://github.com/giusenso)
 * @brief contains robot related data and functions
 * @version 0.1
 * @date 2018-04-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once
#ifndef _ROBOT
#define _ROBOT

#include <iostream>
#include <stdio.h>
#include "occupancy_grid.h"
#include "Tile.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#define     ROBOT_SIZE      32
#define     CLKWISE         -1
#define     ANTI_CLKWISE    +1
#define		ROBOT_PATH		"/tiles/bmp/robot_tile.bmp"

typedef struct Robot {
	SDL_Rect rect;
	int angle;
};

//Key press surfaces constants
enum KeyPressSurfaces
{
	DEFAULT,
	UP,
	DOWN,
	LEFT ,
	RIGHT,
	CLOCKWISE = 90,
	ANTICLOCKWISE = -90,
};

//Function signature_______________________
Robot * createRobot(SDL_Texture* robot_texture, int row, int col, int _angle);
bool killRobot (Robot* rob);
void printRobot(Robot* rob);
void moveRobot(Robot* rob, KeyPressSurfaces key);
void moveRobot1Px(Robot * rob, float dir);
void turnRobot(Robot* rob, int angle, bool animation);
void turnRobot1Degree(Robot* rob, int degree);
void * robotComesToLife(void* args);

#endif
