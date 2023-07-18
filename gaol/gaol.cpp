#include <iostream>
#include <string>
#include <cassert>

#include "gaol.hpp"

extern "C" {
#include "SDL2/SDL.h"
}

static struct _context {
	SDL_Window *window;
	SDL_Surface *screen;
} context;

GAOL::GAOL::GAOL(std::string title) {
	// initialize SDL2
	assert(SDL_Init(SDL_INIT_VIDEO) == 0);

	// create window
	context.window = SDL_CreateWindow(
		title.c_str(), // title of window
		32, 32, // x, y positions
		512, 512, // width, height
		0 // default flags
	);
	assert(context.window != NULL);

	// get surface of window
	context.screen = SDL_GetWindowSurface(context.window);
	assert(context.screen != NULL);

	// fill screen with black pixels
	SDL_FillRect(context.screen, NULL, 0);

	// window is initialized
} // GAOL::GAOL()

GAOL::GAOL::~GAOL() {
	SDL_Quit(); // quit SDL2
} // GAOL::~GAOL()

void GAOL::GAOL::respond() {
	SDL_Event event;
	bool quit = false;

	while (!quit) {
		// poll window events
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					quit = true;
					break;
				default:
					break; // do nothing... for now
			}
		}

		// update state
	}
} // GAOL::respond()
