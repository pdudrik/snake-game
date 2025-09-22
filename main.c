#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "./constants.h"


enum {
	NONE,
	RIGHT,
	LEFT,
	DOWN,
	UP
};


typedef struct Body {
	int x;
	int y;
	int width;
	int height;
	int direction;
	bool is_head;
	struct Body* next;
	struct Body* prev;
} body_s;


typedef struct Food {
	int x;
	int y;
	int size;
} food_s;


bool initiliaze_window();
int get_random_number(int min, int max, int step);
void setup(body_s** snake, food_s* food);
void read_input(SDL_Event event, body_s* snake);
void move(body_s* snake, food_s* food);
body_s* add_body_part(int new_x, int new_y, int direction, bool is_head);
void generate_new_food(body_s* snake, food_s* food);
void update();
void render(body_s* snake, food_s food);
void destroy_window();


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool game_alive = NULL;


int main(int argc, char const *argv[]) {
	srand(time(NULL));

	body_s* snake = NULL;
	snake = (body_s*) malloc(sizeof(body_s));
	food_s food = {};
	
	game_alive = initiliaze_window();
	if (!game_alive) {
		return 0;
	}

	SDL_Event event;
	setup(&snake, &food);
	printf("snake: x=%d, y=%d\n", snake->x, snake->y);
	if (!snake) {printf("Snake is NULL\n"); }

	// Game loop
	while (game_alive) {
		while (SDL_PollEvent(&event) != 0) {
			read_input(event, snake);
		}

		move(snake, &food);
		if (!game_alive) {						// Hit boundary
			break;
		}

		render(snake, food);
		SDL_Delay(150);
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


void setup(body_s** snake, food_s* food) {
	int x = get_random_number(BORDER_THICKNESS,
  						   	  WINDOW_WIDTH - (BORDER_THICKNESS * 2),
							  SNAKE_HEAD_SIZE);
	int y = get_random_number(BORDER_THICKNESS,
							  WINDOW_HEIGHT - (BORDER_THICKNESS * 2),
							  SNAKE_HEAD_SIZE);
	int direction = get_random_number(1, 4, 1);
	(*snake) = add_body_part(x, y, direction, true);

	food->x = get_random_number(BORDER_THICKNESS + SNAKE_HEAD_SIZE,
							   WINDOW_WIDTH-BORDER_THICKNESS*2,
							   SNAKE_HEAD_SIZE);
	food->y = get_random_number(BORDER_THICKNESS + SNAKE_HEAD_SIZE,
							   WINDOW_HEIGHT-BORDER_THICKNESS*2,
							   SNAKE_HEAD_SIZE);

	food->size = SNAKE_HEAD_SIZE / 2;
}


void read_input(SDL_Event event, body_s* snake) {
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
					printf("Pressed A\n");
					if (snake->direction == UP ||
						snake->direction == DOWN) {
						snake->direction = LEFT;
					}
					break;

				case SDLK_w:
					printf("Pressed W\n");
					if (snake->direction == LEFT ||
						snake->direction == RIGHT) {
						snake->direction = UP;
					}
					break;

				case SDLK_d:
					printf("Pressed D\n");
					if (snake->direction == UP ||
						snake->direction == DOWN) {
						snake->direction = RIGHT;
					}
					break;

				case SDLK_s:
					printf("Pressed S\n");
					if (snake->direction == LEFT ||
						snake->direction == RIGHT) {
						snake->direction = DOWN;
					}
					break;
			}
			break;
	}
}


void move(body_s* snake, food_s* food) {
	int prev_x = snake->x;
	int prev_y = snake->y;
	body_s* ptr = snake->next;

	// Shift head
	switch (snake->direction) {
		case RIGHT:
			while (ptr) {
				if ((snake->x + SNAKE_STEP) == ptr->x &&
					snake->y == ptr->y) {
					game_alive = false;
					printf("You hit your tail! GAME OVER\n");
					break;
				}
				ptr = ptr->next;
			}
			snake->x += SNAKE_STEP;
			break;

		case LEFT:
			while (ptr) {
				if ((snake->x - SNAKE_STEP) == ptr->x &&
					snake->y == ptr->y) {
					game_alive = false;
					printf("You hit your tail! GAME OVER\n");
					break;
				}
				ptr = ptr->next;
			}
			snake->x -= SNAKE_STEP;
			break;

		case UP:
			while (ptr) {
				if ((snake->y - SNAKE_STEP) == ptr->y &&
					snake->x == ptr->x) {
					game_alive = false;
					printf("You hit your tail! GAME OVER\n");
					break;
				}
				ptr = ptr->next;
			}
			snake->y -= SNAKE_STEP;
			break;

		case DOWN:
			while (ptr) {
				if ((snake->y + SNAKE_STEP) == ptr->y &&
					snake->x == ptr->x) {
					game_alive = false;
					printf("You hit your tail! GAME OVER\n");
					break;
				}
				ptr = ptr->next;
			}
			snake->y += SNAKE_STEP;
			break;
	}

	// Check for self bite
	// while (ptr) {
	// 	if ((snake->y + SNAKE_STEP) == ptr->y &&
	// 		snake->x == ptr->x) {
	// 		game_alive = false;
	// 		printf("You hit your tail! GAME OVER\n");
	// 		break;
	// 	}
	// 	ptr = ptr->next;
	// }


	ptr = snake->next;
	while (ptr) {
		int temp_x = ptr->x;
		int temp_y = ptr->y;

		ptr->x = prev_x;
		ptr->y = prev_y;

		prev_x = temp_x;				// store x value for next part tail
		prev_y = temp_y;				// store y value for next part tail

		ptr = ptr->next;
	}

	// Check for boundary
	if (snake->x == 0 ||
		snake->x == WINDOW_WIDTH - BORDER_THICKNESS||
		snake->y == 0 ||
		snake->y == WINDOW_HEIGHT - BORDER_THICKNESS) {
		game_alive = false;
		printf("You hit your wall! GAME OVER\n");
	}



	// Add new part of tail if eats food
	if (snake->x == food->x && snake->y == food->y) {
		body_s* tail = snake;
		while (tail->next) {
			tail = tail->next;
		}

		tail->next = add_body_part(prev_x, prev_y, NONE, false);
		generate_new_food(snake, food);
	}
}


void render(body_s* snake, food_s food) {
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

	SDL_Rect food_rect = {food.x + (SNAKE_HEAD_SIZE / 4),
						  food.y + (SNAKE_HEAD_SIZE / 4),
						  food.size,
						  food.size};
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &food_rect);

	while (snake) {
		if (snake->is_head) {
			SDL_Rect snake_head = {snake->x, snake->y, snake->width, snake->height};
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderFillRect(renderer, &snake_head);
		}
		else {
			SDL_Rect snake_head = {snake->x + (snake->width / 6),
								   snake->y + (snake->height / 6),
								   snake->width / 3 * 2,
								   snake->height / 3 * 2};
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderFillRect(renderer, &snake_head);
		}
		snake = snake->next;
	}

	SDL_RenderPresent(renderer);
}


void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


body_s* add_body_part(int new_x, int new_y, int direction, bool is_head) {
	body_s* new_body_part = NULL;
	new_body_part = (body_s*) malloc(sizeof(body_s));

	new_body_part->x = new_x;
	new_body_part->y = new_y;
	new_body_part->width = SNAKE_HEAD_SIZE;
	new_body_part->height = SNAKE_HEAD_SIZE;
	new_body_part->is_head = is_head;
	if (is_head) {
		new_body_part->direction = direction;
	}
	else {
		new_body_part->direction = NONE;
	}
	new_body_part->next = NULL;

	return new_body_part;
}


void generate_new_food(body_s* snake, food_s* food) {
	bool obstacle = true;

	while (obstacle) {
		food->x = get_random_number(BORDER_THICKNESS + SNAKE_HEAD_SIZE,
								   WINDOW_WIDTH-BORDER_THICKNESS*2,
								   SNAKE_HEAD_SIZE);
		food->y = get_random_number(BORDER_THICKNESS + SNAKE_HEAD_SIZE,
								   WINDOW_HEIGHT-BORDER_THICKNESS*2,
								   SNAKE_HEAD_SIZE);

		// Do not generate in snake body
		body_s* ptr = snake->next;
		while (ptr) {
			if (ptr->x == food->x && ptr->y == food->y) {
				obstacle = true;
				break;
			}
			else {
				obstacle = false;
			}

			ptr = ptr->next;
		}
	}
}