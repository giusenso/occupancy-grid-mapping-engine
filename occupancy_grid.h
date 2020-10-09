/**
 * @file occupancy_grid.h
 * @author Giuseppe Sensolini (https://github.com/giusenso)
 * @brief contains general data 
 * @version 0.1
 * @date 2018-04-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Robot.h"
#include "Tile.h"

#define	PI	3.14
#define	TITLE	"Rob the robot"

//::::::::... DATA ...::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//EXTERN____________________
extern SDL_Renderer *rend;
extern SDL_Window *win;

extern SDL_Surface *floor_surface;
extern SDL_Surface *wall_surface;
extern SDL_Surface *unknown_surface;
extern SDL_Surface *robot_surface;

extern SDL_Texture *floor_texture;
extern SDL_Texture *wall_texture;
extern SDL_Texture *unknown_texture;
extern SDL_Texture *robot_texture;


//CONST____________________	

const int WIDTH = 1024;
const int HEIGHT = (WIDTH*3)/4;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const int WINDOW_WIDTH_POSITION = 800;
const int WINDOW_HEIGHT_POSITION = SCREEN_HEIGHT/2 - HEIGHT/2;

const int TILE_SIZE = 32;
const int MAP_WIDTH = WIDTH / TILE_SIZE;
const int MAP_HEIGHT = HEIGHT / TILE_SIZE;

const int STEP_PX = TILE_SIZE / 4;
const int ANIMATION_FRAMES = 4; //frame per animation

const int VISION_DISTANCE = 8;


//Function signature_______________________
bool SDL_init();
void SDL_close();

bool initImgLib();
bool createRenderer();
bool createTexture(SDL_Renderer *rend, SDL_Surface *surf);

int getDistance(int row, int col, SDL_Rect rob);
bool collisionDetection(int tile_row_pos, int tile_col_pos);
void createMap();
void createMap(std::string txt_file_name);
void refreshMap();



