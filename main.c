#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "./constants.h"


enum {
	RIGHT,
	LEFT,
	DOWN,
	UP
};


typedef struct {
	int x;
	int y;
	int width;
	int height;
	int direction;
} head_struct;


typedef struct {
	int x;
	int y;
	int size;
} food_struct;


bool initiliaze_window();
int get_random_number(int min, int max, int step);
void setup();
void read_input(SDL_Event event);
void move(int direction);
void update();
void render();
void destroy_window();


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool game_alive = NULL;
// int last_frame_time = 0;
head_struct head = {};
food_struct food = {};




int main(int argc, char const *argv[]) {
	srand(time(NULL));
	
	game_alive = initiliaze_window();
	if (!game_alive) {
		return 0;
	}

	SDL_Event event;

	setup();
	while (game_alive) {
		while (SDL_PollEvent(&event) != 0) {
			read_input(event);
		}
		move(head.direction);
		// update();
		render(head);
		SDL_Delay(300);
	}

	destroy_window();

	return 0;
}


bool initiliaze_window() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow(WINDOW_TITLE,
							  SDL_WINDOWPOS_CENTERED,
							  SDL_WINDOWPOS_CENTERED,
							  WINDOW_WIDTH,
							  WINDOW_HEIGHT,
							  SDL_WINDOW_SHOWN);

	if (!window) {
		fprintf(stderr, "Error creating SDL Window: %s\n", SDL_GetError());
		return false;
	}
	
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL Renderer: %s\n", SDL_GetError());
		return false;
	}

	return true;
}


int get_random_number(int min, int max, int step) {
	int range = (++max - min) / step;
	int num = rand() % range;

	return num * step + min;
}


void setup() {
	head.x = get_random_number(BORDER_THICKNESS,
							   WINDOW_WIDTH-BORDER_THICKNESS*2,
							   SNAKE_HEAD_SIZE);
	head.y = get_random_number(BORDER_THICKNESS,
							   WINDOW_HEIGHT-BORDER_THICKNESS*2,
							   SNAKE_HEAD_SIZE);
	head.width = SNAKE_HEAD_SIZE;
	head.height = SNAKE_HEAD_SIZE;
	head.direction = 
	printf("HEAD DIRECTION: %d\n", head.direction);

	food.x = get_random_number(BORDER_THICKNESS + (SNAKE_HEAD_SIZE / 4),
							   WINDOW_WIDTH-BORDER_THICKNESS*2,
							   SNAKE_HEAD_SIZE);
	food.y = get_random_number(BORDER_THICKNESS + (SNAKE_HEAD_SIZE / 4),
							   WINDOW_HEIGHT-BORDER_THICKNESS*2,
							   SNAKE_HEAD_SIZE);

	food.size = SNAKE_HEAD_SIZE / 2;
}


void read_input(SDL_Event event) {
	switch (event.type) {
		case SDL_QUIT:
			game_alive = false;
			break;

		case SDL_KEYDOWN:

			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					game_alive = false;
					break;

				case SDLK_a:
					head.direction = LEFT;
					break;

				case SDLK_w:
					head.direction = UP;
					break;

				case SDLK_d:
					head.direction = RIGHT;
					break;

				case SDLK_s:
					head.direction = DOWN;
					break;
			}
			break;
	}
}


void move(int direction) {
	// float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;
	// last_frame_time = SDL_GetTicks();
	// head.x += SNAKE_STEP * delta_time;
	// head.y += SNAKE_STEP * delta_time;

	// float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;
	// last_frame_time = SDL_GetTicks();
	
	switch (direction) {
		case RIGHT:		
			// head.x += SNAKE_STEP * delta_time;
			head.x += SNAKE_STEP;
			break;

		case LEFT:
			head.x -= SNAKE_STEP;
			// head.x -= SNAKE_STEP * delta_time;
			break;

		case UP:
			head.y -= SNAKE_STEP;
			// head.y -= SNAKE_STEP * delta_time;
			break;

		case DOWN:
			head.y += SNAKE_STEP;
			// head.y += SNAKE_STEP * delta_time;
			break;
	}

	printf("x: %d, y: %d\n", head.x, head.y);
	if (head.x == 0 ||
		head.x == WINDOW_WIDTH-BORDER_THICKNESS ||
		head.y == 0 ||
		head.y == WINDOW_HEIGHT-BORDER_THICKNESS*2) {
		game_alive = false;
		printf("you lost");
	}
}


void update() {
	// SDL_Delay()
	// int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
	// if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
	// 	SDL_Delay(time_to_wait);
	// }

	// float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;
	// last_frame_time = SDL_GetTicks();

	// head.x += 90 * delta_time;
	// head.y += 60 * delta_time;

	// printf("head.x = %f, head.y = %f\n", head.x, head.y);
	// printf("delta_time = %lf\n", delta_time);
	// printf("last_frame_time = %d\n", last_frame_time);
	// printf("SNAKE_STEP = %d\n\n", SNAKE_STEP);
}


void render() {
	SDL_RenderClear(renderer);

	SDL_Rect borders = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &borders);


	SDL_Rect playing_board = {BORDER_THICKNESS,
							  BORDER_THICKNESS,
							  WINDOW_WIDTH-(BORDER_THICKNESS*2),
							  WINDOW_HEIGHT-BORDER_THICKNESS*2};
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &playing_board);

	SDL_Rect food_rect = {food.x, food.y, food.size, food.size};
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &food_rect);

	printf("FOOD: x=%d, y=%d\n", food.x, food.y);

	// SDL_Rect ball_rect = {(int) head.x, (int) head.y, head.width, head.height};
	SDL_Rect snake_head = {head.x, head.y, head.width, head.height};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &snake_head);

	SDL_RenderPresent(renderer);
}


void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}