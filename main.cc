#include <iostream>
#include <SDL2/SDL.h>
#include <chrono>

#include "utils.hpp"

const int32_t WIDTH = 800;
const int32_t HEIGHT = 600;

const int32_t BLOCK_WIDTH = 20;
const int32_t BLOCK_HEIGHT = 20;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

std::vector<SDL_Rect> snake;
SDL_Rect food = {utils::generate_random(0, 38) * BLOCK_WIDTH, utils::generate_random(0, 28) * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT};

bool left = false,
	 right = false, 
	 up = false,
	 down = false;

bool running = false;

bool init(void)
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Failed to initialize SDL2, " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if(!window)
    {
        std::cout << "Failed to create a window, " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
    {
        std::cout << "Failed to create a renderer, " << SDL_GetError() << std::endl;
        return false;
    }

	snake.push_back({(WIDTH / 2), (HEIGHT / 2), BLOCK_WIDTH, BLOCK_HEIGHT});
    
	return true;
}

void render()
{

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
	
	for(auto &x : snake)
	{
		SDL_RenderFillRect(renderer, &x);
	}

	SDL_SetRenderDrawColor(renderer, 255, 54, 44, 255);
	SDL_RenderFillRect(renderer, &food);

	// Grid effect
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

	for(int32_t y = BLOCK_HEIGHT; y < HEIGHT; y += BLOCK_HEIGHT)
	{
		SDL_RenderDrawLine(renderer, 0, y, WIDTH, y);	
	}

	for(int32_t x = BLOCK_WIDTH; x < WIDTH; x += BLOCK_WIDTH)
	{
		SDL_RenderDrawLine(renderer, x, 0, x, HEIGHT);
	}
	
	SDL_RenderPresent(renderer);
}

void handle_events(SDL_Event &event)
{
	SDL_PollEvent(&event);
	
	if(event.type == SDL_QUIT) running = false;

	switch(event.key.keysym.sym)
	{
		case SDLK_UP:
			up = true;
			left = right = down = false;
			break;

		case SDLK_DOWN:
			down = true;
			left = right = up = false;
			break;

		case SDLK_LEFT:
			left = true;
			right = up = down = false;
			break;

		case SDLK_RIGHT:
			right = true;
			left = up = down = false;
			break;
	}
}

void update()
{
	static auto start = std::chrono::high_resolution_clock::now();
	auto current = std::chrono::high_resolution_clock::now();
	SDL_Rect tmp;

	if(std::chrono::duration_cast<std::chrono::milliseconds>(current - start).count() >= 100)
	{
		for(size_t i = snake.size(); i > 0; i--)
		{
			snake[i].y = snake[i - 1].y;
			snake[i].x = snake[i - 1].x;
		}	
	
		if(up)
		{
			snake[0].y -= BLOCK_HEIGHT;
		}

		if(down)
		{
			snake[0].y += BLOCK_HEIGHT;
		}

		if(left)
		{ 
			snake[0].x -= BLOCK_WIDTH;
			
		}

		if(right)
		{
			snake[0].x += BLOCK_WIDTH;
		}

		start = std::chrono::high_resolution_clock::now();
	
	}

	if(snake[0].x == food.x && snake[0].y == food.y)
	{
		snake.push_back({snake[snake.size() - 1].x, snake[snake.size() - 1].y, BLOCK_WIDTH, BLOCK_HEIGHT});
		food.x = utils::generate_random(0, 38) * BLOCK_WIDTH;
		food.y = utils::generate_random(0, 28) * BLOCK_HEIGHT;
	}

	// if(left)
	// {
	// 	 if(snake[0].x == food.x && snake[0].y == food.y)
	// 	 {
	// 		snake.push_back({snake[snake.size() - 1].x + 20, snake[snake.size() - 1].y, BLOCK_WIDTH, BLOCK_HEIGHT});
	// 		food.x = utils::generate_random(0, 38) * BLOCK_WIDTH;
	// 		food.y = utils::generate_random(0, 28) * BLOCK_HEIGHT;
	// 	 }
	// }
	// 
	// if(up)
	// {
	// 	if(snake[0].x == food.x && snake[0].y == food.y)
	// 	{
	// 		snake.push_back({snake[snake.size() - 1].x, snake[snake.size() - 1].y + 20, BLOCK_WIDTH, BLOCK_HEIGHT});
	// 		food.x = utils::generate_random(0, 38) * BLOCK_WIDTH;
	// 		food.y = utils::generate_random(0, 28) * BLOCK_HEIGHT;
	// 	}
	// }
	// 
	// if(down)
	// {
	// 	if(snake[0].x == food.x && snake[0].y == food.y)
	// 	{
	// 		snake.push_back({snake[snake.size() - 1].x, snake[snake.size() - 1].y - 20, BLOCK_WIDTH, BLOCK_HEIGHT});
	// 		food.x = utils::generate_random(0, 38) * BLOCK_WIDTH;
	// 		food.y = utils::generate_random(0, 28) * BLOCK_HEIGHT;
	// 	}
	// }
	// 
	// if(right)
	// {
	// 	if(snake[0].x == food.x && snake[0].y == food.y)
	// 	{
	// 		snake.push_back({snake[snake.size() - 1].x - 20, snake[snake.size() - 1].y, BLOCK_WIDTH, BLOCK_HEIGHT});
	// 		food.x = utils::generate_random(0, 38) * BLOCK_WIDTH;
	// 		food.y = utils::generate_random(0, 28) * BLOCK_HEIGHT;
	// 	}
	// }

	if(snake[0].x >= WIDTH)
	{
		snake[0].x = 0;
	}

	if(snake[0].x + 20 <= 0)
	{
		snake[0].x = WIDTH - 20;
	}

	if(snake[0].y >= HEIGHT)
	{
		snake[0].y = 0;
	}

	if(snake[0].y + 20 <= 0)
	{
		snake[0].y = HEIGHT - 20;
	}

	std::cout << snake.size() << std::endl;
}

int32_t main(void)
{
	running = init();
    SDL_Event event;

	while(running)
	{
		render();
		handle_events(event);
		update();
	}

	return EXIT_SUCCESS;
}
