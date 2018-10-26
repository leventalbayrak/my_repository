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

#include "Twitch_core.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{
	//initialize network
	Twitch::startup();

	const char *username = "plogp";
	//get this token from https://twitchapps.com/tmi/ and keep it safe
	//remove the oauth: part when you copy paste
	const char *token = "y1i15zeitv1sucg8y3pbd5lz4zzozx";

	//init connection data
	Twitch::Connection connection;
	Twitch::init(&connection, username, token);
	
	//make TCP connection to twitch and login
	Twitch::connect(&connection);

	//join a channel
	//pass channel name
	printf("connecting to channels\n");
	Twitch::join_Channel(&connection, "plogp");
	

	//initialize message table
	//will contaain incoming message list from all connected channels
	Twitch::Message::Table incoming;
	Twitch::Message::init(&incoming);

	//SDL
	SDL_Init(SDL_INIT_VIDEO);

	SDL_ShowCursor(SDL_DISABLE);

	int screen_width = 800;
	int screen_height = 600;

	SDL_Window *window = SDL_CreateWindow(
		"twitch bot",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED);

	SDL_Surface *surface = IMG_Load("mario.png");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	float size = 32;
	float px = screen_width / 2;
	float py = screen_height - size;
	float vx = 8;
	float vy = 8;

	bool done = false;
	printf("chat log\n");
	while(!done)
	{
		unsigned int timestamp = clock();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}
		
		//collect all messages from all channels
		incoming.n_count = 0;
		Twitch::communicate(&incoming, &connection, timestamp);
		if (connection.active == false)
		{
			printf("connection was closed!\n");
			break;
		}

		//print received messages from all channels
		for (int i = 0; i < incoming.n_count; i++)
		{
			if (strstr(incoming.message[i], "!left"))
			{
				px += -vx;
				printf("x pos = %f\n", px);
			}
			if (strstr(incoming.message[i], "!right"))
			{
				px += vx;
				printf("x pos = %f\n", px);
			}
			if (strstr(incoming.message[i], "!up"))
			{
				py += -vy;
				printf("y pos = %f\n", py);
			}
			if (strstr(incoming.message[i], "!down"))
			{
				py += vy;
				printf("y pos = %f\n", py);
			}
		}

		//DRAW
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		SDL_Rect src_rect = { 0,0,24,24};
		SDL_Rect dest_rect = { screen_width / 2,screen_height/2,size, size};
		//SDL_RenderCopy(renderer, texture, &src_rect, &dest_rect);
		SDL_SetTextureAlphaMod(texture, 255);
		SDL_RenderCopyEx(renderer, texture, &src_rect, &dest_rect, 30, NULL, SDL_FLIP_NONE);
		
		dest_rect.x += size/2;
		SDL_SetTextureAlphaMod(texture, 128);
		SDL_RenderCopyEx(renderer, texture, &src_rect, &dest_rect, 30, NULL, SDL_FLIP_NONE);

		SDL_RenderPresent(renderer);
	}
	
	return 0;
	
}