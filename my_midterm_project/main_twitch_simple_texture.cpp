#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2_image-2.0.3\\lib\\x86\\SDL2_image.lib")

#pragma comment(linker,"/subsystem:console")

#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{

	//SDL
	SDL_Init(SDL_INIT_VIDEO);

	SDL_ShowCursor(SDL_DISABLE);

	int screen_width = 960;
	int screen_height = 768;

	SDL_Window *window = SDL_CreateWindow(
		"twitch bot",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED);

	SDL_Surface *surface = IMG_Load("tileset.png");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	float size = 32;

	int **tilemap = new int*[24];
	for (int i = 0; i < 24; i++)
	{
		tilemap[i] = new int[30];
		memset(tilemap[i], 0, sizeof(int) * 30);
	}
	
	for (int i = 0; i < 20; i++)
	{
		tilemap[rand() % 24][rand() % 30] = 1 + rand() % 2;
	}

	bool done = false;

	while(!done)
	{

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}
		
		
		//DRAW
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		//SDL_RenderCopy(renderer, texture, &src_rect, &dest_rect);
		SDL_SetTextureAlphaMod(texture, 255);

		for (int i = 0; i < 24; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				SDL_Rect src_rect;
				switch (tilemap[i][j])
				{
				case 0: 
					src_rect.x = 0;
					src_rect.y = 0;
					break;
				case 1:
					src_rect.x = 64;
					src_rect.y = 0;
					break;
				case 2:
					src_rect.x = 0;
					src_rect.y = 32;
					break;
				}

				src_rect.w = size;
				src_rect.h = size;

				SDL_Rect dest_rect;
				dest_rect.x = j * size;
				dest_rect.y = i * size;
				dest_rect.w = size;
				dest_rect.h = size;

				SDL_RenderCopy(renderer, texture, &src_rect, &dest_rect);
			}
		}
		

		SDL_RenderPresent(renderer);
	}
	
	return 0;
	
}