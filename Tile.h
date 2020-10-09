/**
 * @file Tile.h
 * @author Giuseppe Sensolini (https://github.com/giusenso)
 * @brief contains tile related data
 * @version 0.1
 * @date 2018-04-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once 

#define		FLOOR_PATH		"/tiles/bmp/floor_tile.bmp"
#define		WALL_PATH		"/tiles/bmp/wall_tile.bmp"
#define		UNKNOWN_PATH	"/tiles/bmp/unknown_tile.bmp"

#define		TOTAL_TILES		MAP_WIDTH*MAP_HEIGHT

typedef enum tileType {
	FLOOR,
	WALL
};

typedef struct Tile {
	SDL_Rect rect;
	tileType type;
	bool known;
};

