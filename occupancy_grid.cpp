/**
 * @file occupancy-grid.cpp
 * @author Giuseppe Sensolini (https://github.com/giusenso)
 * @brief Sketch of low-level graphic engine: Occupancy Grid Mapping for robot navigation task
 * @version 0.3
 * @date 2018-04-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

//::::::::... LIBS ...:::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#include <iostream>
#include <errno.h>
#include <fstream>
#include <pthread.h>
#include "occupancy_grid.h"


#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

//::::::::... DATA ...:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
bool close_requested = false;
bool live = false;

SDL_Renderer *rend = NULL;
SDL_Window *win = NULL;

SDL_Surface *floor_surface = NULL;
SDL_Surface *wall_surface = NULL;
SDL_Surface *unknown_surface = NULL;
SDL_Surface *robot_surface = NULL;

SDL_Texture *floor_texture = NULL;
SDL_Texture *wall_texture = NULL;
SDL_Texture *unknown_texture = NULL;
SDL_Texture *robot_texture = NULL;

//int rob_row; int rob_col;

// Tile struct matrix
struct Tile * tile[MAP_HEIGHT][MAP_WIDTH];

// Robot struct
struct Robot * robot;

int i = 0;
int next_x_pos = 0;
int next_y_pos = 0;



//:::::::... M A I N ...:::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main(int argc, char *argv[]) {

	srand((unsigned)time(NULL));

	// inizialize window
	if (!SDL_init()) {
		std::cout << "Init() Error." << std::endl;
		SDL_close();
	}

	// inizialize library for picture handling
		if (!initImgLib()) {
		std::cout << "initializeImgLib() Error." << std::endl;
		SDL_close();
	}

	// create renderer
	if (!createRenderer()) {
		std::cout << "createRender() Error." << std::endl;
		SDL_close();
	}

	// create surfaces
	robot_surface = SDL_GetWindowSurface(win);
	floor_surface = SDL_GetWindowSurface(win);
	wall_surface = SDL_GetWindowSurface(win);
	unknown_surface = SDL_GetWindowSurface(win);


	// get current directory
	char current_path[FILENAME_MAX];
	if (!GetCurrentDir(current_path, sizeof(current_path))){
		exit(EXIT_FAILURE);
	}
	current_path[sizeof(current_path) - 1] = '\0'; /* not really required */
	char tmp_path[FILENAME_MAX];

	// load tile media
	memcpy(tmp_path, current_path, sizeof(current_path));
	floor_surface = IMG_Load(strcat(tmp_path,FLOOR_PATH));
	if (floor_surface == NULL) {
		std::cout << "SDL #4a floor_surface loading error\n" << SDL_GetError() << "\n" << IMG_GetError() << std::endl;
		return 1;
	}
	memcpy(tmp_path, current_path, sizeof(current_path));
	wall_surface = IMG_Load(strcat(tmp_path,WALL_PATH));
	if (wall_surface == NULL) {
		std::cout << "SDL #4b wall_surface loading error\n" << SDL_GetError() << "\n" << IMG_GetError() << std::endl;
		return 1;
	}
	memcpy(tmp_path, current_path, sizeof(current_path));
	unknown_surface = IMG_Load(strcat(tmp_path,UNKNOWN_PATH));
	if (unknown_surface == NULL) {
		std::cout << "SDL #4c unknown_surface loading error\n" << SDL_GetError() << "\n" << IMG_GetError() << std::endl;
		return 1;
	}
	// load robot media
	memcpy(tmp_path, current_path, sizeof(current_path));
	robot_surface = IMG_Load(strcat(tmp_path,ROBOT_PATH));
	if (robot_surface == NULL) {
		std::cout << "SDL #4d robot_surface loading error\n" << SDL_GetError() << "\n" << IMG_GetError() << std::endl;
		return 1;
	}

	// crea texture per le tile 
	floor_texture = SDL_CreateTextureFromSurface(rend, floor_surface);
	wall_texture = SDL_CreateTextureFromSurface(rend, wall_surface);
	unknown_texture = SDL_CreateTextureFromSurface(rend, unknown_surface);

	// crea texture per il robot
	robot_texture = SDL_CreateTextureFromSurface(rend, robot_surface);


	// Robot settings
	std::cout << "spawning new Robot... ";
	robot = createRobot(robot_texture, MAP_HEIGHT*TILE_SIZE/1.5, MAP_WIDTH*TILE_SIZE/2, 270);
	printRobot(robot);
	std::cout << "Done." << std::endl;


	//----------------------------------------------

	//initialize map
	createMap("map_3.txt");

	//draw image to the window
	refreshMap();
	
	SDL_RenderPresent(rend);


	//prepare args for robot_comes_alive_thread
	int ret2 = 0;
	pthread_t robot_thread;

	//animation loop:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	while (!close_requested) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				close_requested = true;
			}

			switch (event.type){
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {

						// MOVE COMMANDS___________________________
					case SDLK_w:
						std::cout << "UP Pressed\n";
						if (robot->angle != 0 && robot->angle != 180)
							moveRobot(robot, UP);
						break;

					case SDLK_s:
						std::cout << "DOWN Pressed\n";
						if (robot->angle != 0 && robot->angle != 180)
							moveRobot(robot, DOWN);
						break;

					case SDLK_a:
						std::cout << "LEFT Pressed\n";
						if (robot->angle != 90 && robot->angle != 270)
							moveRobot(robot, LEFT);
						break;

					case SDLK_d:
						std::cout << "RIGHT Pressed\n";
						if (robot->angle != 90 && robot->angle != 270)
							moveRobot(robot, RIGHT);
						break;

						// TURN COMMANDS___________________________
					case SDLK_LEFT:
						if (robot->angle != 180) {
							std::cout << "Turn left Pressed\n";
							robot->angle = 180;
						}
						break;

					case SDLK_RIGHT:
						if (robot->angle != 0) {
							std::cout << "Turn right Pressed\n";
							robot->angle = 0;
						}
						break;

					case SDLK_UP:
						if (robot->angle != 270) {
							std::cout << "Turn up Pressed\n";
							robot->angle = 270;
						}
						break;

					case SDLK_DOWN:
						if (robot->angle != 90) {
							std::cout << "turn down Pressed\n";
							robot->angle = 90;
						}
						break;

					//AUTO PILOTE KEY___________________________________

					/*	multithread version need to be fixed
					*	using SDL api, should be some functions 
					*	to properly handle threads routine
					*/
					case SDLK_SPACE:
						if (!live){
							ret2 = pthread_create(	&robot_thread,
													NULL,
													&robotComesToLife,
													(void*)robot	);
							if(ret2 != 0 ){
								perror("pthread_create() ERROR!\n");
								return -1;
							}
						}
						else{
							ret2 = pthread_detach(robot_thread);
							if ( ret2 != 0){
								perror("pthread_detach() ERROR\n");
								return -1;
							}
							live = true;
						}
						break;
		
						robotComesToLife((void*)robot);
						break;
					
					}
			}	

			SDL_RenderClear(rend);

			refreshMap();

			SDL_RenderPresent(rend);

		}

		//wait 1/60th of a second
		SDL_Delay(1000 / 60);
	}
	//:::::::::::::::::::::::::::::::::::::::

	SDL_close();
	return 0;
}



//::::::::... FUNCTIONS ...::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
 * @brief initialize SDL
 * 
 * @return true if initialized correctly
 * @return false if errors occur
 */
bool SDL_init(){

	//Initialization flag
	bool success = true;

	//Start SDL _________________________________________________________
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		std::cout << "SDL 1# initializing error\n" << SDL_GetError() << std::endl;
		success = false; return success;
	}
	else {
		//Create window
		win = SDL_CreateWindow(TITLE, WINDOW_WIDTH_POSITION, WINDOW_HEIGHT_POSITION, WIDTH, HEIGHT, 0);

		if (win == NULL) {
			std::cout << "SDL 2# creating windows error\n" << SDL_GetError() << std::endl;
			success = false;
		}
	}
	return success;
}

/**
 * @brief Close every SDL object
 * 
 */
void SDL_close(){

	//Destroy texture;
	SDL_DestroyTexture(robot_texture);
	robot_texture = NULL;

	//Destroy renderer
	SDL_DestroyRenderer(rend);
	rend = NULL;

	//Deallocate surface
	SDL_FreeSurface(robot_surface);
	robot_surface = NULL;

	//Destroy window
	SDL_DestroyWindow(win);
	win = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

/**
 * @brief initialize the SDL_image lib
 * 
 * @return true if initialized correctly
 * @return false errors occur
 */
bool initImgLib() {
	/*per utilizzarla serve:
	1)	la lib SDL_image
	2)	i file .dll (che si trovano nella lib) devono essere copiati
	nella directory dell'exe
	*/

	bool success = true;

	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	if (initted&flags != flags) {
		std::cout << "IMG_Init error\n" << SDL_GetError() << IMG_GetError();
		success = false;
	}
	return success;
}

/**
 * @brief Create a Renderer object
 * 
 * @return true created correctly
 * @return false an error occurs
 */
bool createRenderer() {

	bool success = true;

	// create a renderer: setup graphic harware
	Uint32 render_flag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	rend = SDL_CreateRenderer(win, -1, render_flag); //associa questo renderer a win
	if (!rend) {
		std::cout << "SDL 3a# create robot renderer error\n" << SDL_GetError();
		success = false;
	}

	SDL_SetRenderDrawColor(rend, 29, 152, 149, 255);
	SDL_RenderClear(rend);
	return success;
}

int getDistance(int row, int col, SDL_Rect rob) {
	return sqrt( pow(((rob.x / TILE_SIZE)-col) , 2) + pow(((rob.y / TILE_SIZE)-row) , 2));
};

bool collisionDetection(int tile_row_pos, int tile_col_pos) {
	if (tile_row_pos<0 || tile_row_pos>MAP_HEIGHT - 1 ||
		tile_col_pos<0 || tile_col_pos>MAP_WIDTH - 1 ||
		tile[tile_row_pos][tile_col_pos]->type == WALL) {
		return true;
	}
	return false;
}

/**
 * @brief Create a Map object based on the default map
 * 
 */
void createMap(){

	std::ofstream outfile("maps/map_1.txt");
	if (!outfile.is_open()) std::cerr << "Couldn't open 'output.txt'" << std::endl;
	outfile << MAP_HEIGHT << std::endl;
	outfile << MAP_WIDTH << std::endl;

	SDL_Rect tile_rect;
	tile_rect.h = tile_rect.w = TILE_SIZE;
	tile_rect.x = tile_rect.y = 0;

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {

			tile[i][j] = (Tile*)malloc(sizeof(struct Tile));

			tile[i][j]->rect = tile_rect;
			tile[i][j]->rect.x = TILE_SIZE * j;
			tile[i][j]->rect.y = TILE_SIZE * i;

			tile[i][j]->known = false; //inizializzo a not known

			tile[i][j]->type = FLOOR;

			//set wall
			if (i == 7 && j > (robot->rect.x / TILE_SIZE) - 2) tile[i][j]->type = WALL;

			if (i == robot->rect.y / TILE_SIZE && j == robot->rect.x / TILE_SIZE) {
				tile[i][j]->type = FLOOR;
				tile[i][j]->known = true;
			}

			if (tile[i][j]->type == FLOOR) outfile << "0 ";
			if (tile[i][j]->type == WALL) outfile << "= ";
			if (j == MAP_WIDTH-1) outfile << std::endl;		
		}
	}
	outfile.close();
	std::cerr << "Map file successfully created" << std::endl;
}

/**
 * @brief Create a Map object based on a .txt map
 * 
 * @param txt_file_name 
 */
void createMap(std::string txt_file_name) {

	std::ifstream map_stream;
	map_stream.open("maps/"+txt_file_name);
	if (!map_stream.is_open()) std::cerr << "Couldn't open 'output.txt'" << std::endl;

	std::string rows, cols; char ch;
	map_stream >> rows; 
	map_stream >> cols;
	if (std::stoi(rows) != MAP_WIDTH && std::stoi(cols) != MAP_WIDTH)
		std::cout << "Errore: le dimensione della mappa nel txt non sono corrette" << std::endl;

	SDL_Rect tile_rect;
	tile_rect.h = tile_rect.w = TILE_SIZE;
	tile_rect.x = tile_rect.y = 0;

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {

			tile[i][j] = (Tile*)malloc(sizeof(struct Tile));

			tile[i][j]->rect = tile_rect;
			tile[i][j]->rect.x = TILE_SIZE * j;
			tile[i][j]->rect.y = TILE_SIZE * i;

			tile[i][j]->known = false; //inizializzo known
			
			map_stream >> ch;
			switch (ch) {
			case ' ': 
				map_stream >> ch;
			case '0' : 
				tile[i][j]->type = FLOOR;
				break;
			case  '=' :
				tile[i][j]->type = WALL;
				break;
			}

			if (i == robot->rect.y / TILE_SIZE && j == robot->rect.x / TILE_SIZE) {
				tile[i][j]->type = FLOOR;
				tile[i][j]->known = true;
			}

		}
	}
	map_stream.close();
	std::cerr << "Map file successfully created" << std::endl;
}

/**
 * @brief refresh every element in the map
 * 
 */
void refreshMap() {

	int rob_row = robot->rect.y / TILE_SIZE;
	int rob_col = robot->rect.x / TILE_SIZE;

	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {

			//Algo: calc vision field
			if (!tile[row][col]->known) {

				if (getDistance(row, col, robot->rect) < VISION_DISTANCE) {
					tile[row][col]->known = true;
				}
				if ((row == rob_row && (col == rob_col + VISION_DISTANCE || col == rob_col - VISION_DISTANCE))) {
					tile[row][col]->known = true;
				}
				if ((col == rob_col && (row == rob_row + VISION_DISTANCE || row == rob_row - VISION_DISTANCE))) {
					tile[row][col]->known = true;
				}

				if (i == TOTAL_TILES-1) std::cout << i << "\n\n*** Congratulation! *** \nYOU KNOW ALL TILES!\n" << std::endl;

			}

			//draw tiles
			if (!(tile[row][col]->known)) {
				SDL_RenderCopy(rend, unknown_texture, NULL, &tile[row][col]->rect);
			}
			else{
				if (tile[row][col]->type == FLOOR) {
					SDL_RenderCopy(rend, floor_texture, NULL, &tile[row][col]->rect);
				}
			}
		}
	}
	
	//draw wall
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			if (tile[row][col]->type == WALL && tile[row][col]->known) {
				SDL_RenderCopy(rend, wall_texture, NULL, &tile[row][col]->rect);
			}
		}
	}

	//draw robot
	SDL_RenderCopyEx(rend, robot_texture, NULL, &(robot->rect), robot->angle , NULL, SDL_FLIP_NONE);

}
