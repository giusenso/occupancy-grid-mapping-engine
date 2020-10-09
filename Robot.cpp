/**
 * @file Robot.cpp
 * @author Giuseppe Sensolini (https://github.com/giusenso)
 * @brief contains robot related data and functions
 * @version 0.1
 * @date 2018-04-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "Robot.h"

/**
 * @brief Create a Robot object
 * 
 * @param robot_texture 
 * @param row starting row
 * @param col starting col
 * @param _angle starting angle
 * @return Robot* 
 */
Robot * createRobot(SDL_Texture* robot_texture,
					int row, int col, int _angle){
	
	SDL_Rect rect;
	//initialize robot position
	rect.h = ROBOT_SIZE; 
	rect.w = ROBOT_SIZE;
	rect.x = col;
	rect.y = row;

	struct Robot * rob = (Robot*) malloc(sizeof(Robot));
	rob->rect = rect;
	rob->angle = _angle; //default will be 270

	//get the dimension of the texture
	SDL_QueryTexture(robot_texture, NULL, NULL, &(rob->rect.h), &(rob->rect.h));

	return rob;
}

/**
 * @brief destroy robot object
 * 
 * @param rob 
 * @return true 
 * @return false 
 */
bool killRobot(Robot* rob){
	free(rob);
	return true;
}

/**
 * @brief print robot status
 * 
 * @param rob 
 */
void printRobot(Robot * rob){
	printf("  \n\n==================\
				\n__ Robot status __\n\
				\n     x = %d      \
				\n     y = %d      \
				\n   angle = %d    \ 
				\n==================\n\n",
				rob->rect.x, rob->rect.y, rob->angle);
}

/**
 * @brief 
 * 
 * @param rob 
 * @param key 
 */
void moveRobot(Robot* rob, KeyPressSurfaces key) {

	int i = 1;
	switch (key) {
	case RIGHT:
		i = 1;
		if (!collisionDetection(rob->rect.y/TILE_SIZE, (rob->rect.x + rob->rect.w)/TILE_SIZE)) {

			while (i <= ANIMATION_FRAMES) {
				rob->rect.x += rob->rect.w / ANIMATION_FRAMES;

				SDL_RenderClear(rend);

				refreshMap();
				SDL_RenderPresent(rend);

				SDL_Delay(1000/60);	i++;
			}
		}
		break;

	case LEFT:
		i = 1;
		if (!collisionDetection(rob->rect.y / TILE_SIZE, (rob->rect.x - rob->rect.w) / TILE_SIZE)) {
			while (i <= ANIMATION_FRAMES) {
				rob->rect.x -= rob->rect.w / ANIMATION_FRAMES;

				SDL_RenderClear(rend);

				refreshMap();
				SDL_RenderPresent(rend);
				SDL_Delay(1000 / 60);
				i++;
			}
		}
		break;

	case UP:
		i = 1;
		if (!collisionDetection((rob->rect.y - rob->rect.h) / TILE_SIZE, rob->rect.x / TILE_SIZE)) {
			while (i <= ANIMATION_FRAMES) {
				rob->rect.y -= rob->rect.h / ANIMATION_FRAMES;
		
				SDL_RenderClear(rend);

				refreshMap();
				SDL_RenderPresent(rend);
				SDL_Delay(1000 / 60);
				i++;
			}
		}
		break;

	case DOWN:
		i = 1;
		if (!collisionDetection((rob->rect.y + rob->rect.h) / TILE_SIZE, rob->rect.x / TILE_SIZE)) {
			while (i <= ANIMATION_FRAMES) {
				rob->rect.y += rob->rect.h / ANIMATION_FRAMES;

				SDL_RenderClear(rend);

				refreshMap();
				SDL_RenderPresent(rend);
				SDL_Delay(1000 / 60);
				i++;
			}
		}
		break;
	}


}

/**
 * @brief 
 * 
 * @param rob 
 * @param dir 
 */
void moveRobot1Px(Robot* rob, float dir){
    rob->rect.x += (int) cos(dir*(PI/180));
	rob->rect.y += (int) sin(dir*(PI/180));
}

/**
 * @brief rotate robot object
 * 
 * @param rob 
 * @param degree 
 * @param animation
 */
void turnRobot(Robot* rob, int degree, bool animation) {

	if (animation) {
		int i = 0;
		while (i < 5) {
			rob->angle += degree / 5;

			SDL_RenderClear(rend);
			refreshMap();
			SDL_RenderPresent(rend);
			SDL_Delay(1000 / 60);
			i++;
		}
	}
	else rob->angle += degree;

	if (rob->angle >= 360) rob->angle -= 360;
	if (rob->angle <= -90) rob->angle += 360;

	std::cout << "View rob->angle = " << rob->angle << std::endl;
}


void turnRobot1Degree(Robot* rob, int degree){
	rob->angle += degree;
}


void * robotComesToLife(void* args) {
	struct Robot * rob = (Robot*)args;
	int live = true;

	while (live) {
		switch (rob->angle) {
		case 0:
			//int next_x_pos = (rob->rect.x + TILE_SIZE * 2);

			if (!collisionDetection(rob->rect.y / TILE_SIZE, (rob->rect.x + TILE_SIZE * 2) / TILE_SIZE)) {
				moveRobot(rob, RIGHT);
			}
			else {
				if (rand() % 2 == 0)turnRobot(rob, ANTICLOCKWISE, true);
				else turnRobot(rob, CLOCKWISE, true);
			}
			break;
		case 90:
			//int next_y_pos = (rob->rect.y + TILE_SIZE * 2);

			if (!collisionDetection((rob->rect.y + TILE_SIZE * 2) / TILE_SIZE, rob->rect.x / TILE_SIZE)) {
				moveRobot(rob, DOWN);
			}
			else {
				if (rand() % 2 == 0)turnRobot(rob, ANTICLOCKWISE, true);
				else turnRobot(rob, CLOCKWISE, true);
			}
			break;

		case 180:
			//int next_x_pos = (rob->rect.x - TILE_SIZE * 2);

			if (!collisionDetection(rob->rect.y / TILE_SIZE, (rob->rect.x - TILE_SIZE * 2) / TILE_SIZE)) {
				moveRobot(rob, LEFT);
			}
			else {
				if (rand() % 2 == 0)turnRobot(rob, ANTICLOCKWISE, true);
				else turnRobot(rob, CLOCKWISE, true);
			}
			break;

		case 270:
			//int next_y_pos = (rob->rect.y - TILE_SIZE*2);

			if (!collisionDetection((rob->rect.y - TILE_SIZE * 2) / TILE_SIZE, rob->rect.x / TILE_SIZE)) {
				moveRobot(rob, UP);
			}
			else {
				if (rand() % 2 == 0)turnRobot(rob, ANTICLOCKWISE, true);
				else turnRobot(rob, CLOCKWISE, true);
			}
			break;

		}

		SDL_Delay(300);
    }
}