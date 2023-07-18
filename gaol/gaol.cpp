#include <string>
#include <vector>

#include "gaol.hpp"

#include <iostream>
#include <cassert>

extern "C" {
#include "SDL2/SDL.h"
}

static struct _context {
	SDL_Window *window;
	SDL_Surface *screen;
} context;

GAOL::GAOL::GAOL(std::string title) {
	// initialize SDL2
	assert(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) == 0);

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

	// window is initialized
} // GAOL::GAOL()

GAOL::GAOL::~GAOL() {
	SDL_Quit(); // quit SDL2
} // GAOL::~GAOL()

void GAOL::GAOL::respond(void (*callback)(void *state, double delta), void *state) {
	SDL_Event event;
	bool quit = false;
	int prev = SDL_GetTicks(), now;
	double delta;

	while (!quit) {
		// poll window events
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					quit = true;
					break;
				default:
					break; // do nothing
			}
		}

		now = SDL_GetTicks();
		delta = (now - prev) / 1000.0;
		prev = now;

		// update state
		callback(state, delta);

		// fill screen with black pixels
		SDL_FillRect(context.screen, NULL, 0);

		// display all objects
		for (size_t i = 0; i < objects.size(); i++) {
			Object *o = objects[i];

			// create a rectangle representing this object's position
			SDL_Rect rect;
			rect.x = (int)o->getX() - 16;
			rect.y = (int)o->getY() - 16;
			rect.w = 32;
			rect.h = 32;

			// fill in this rectangle on the screen
			SDL_FillRect(context.screen, &rect,
				SDL_MapRGB(context.screen->format, 128, 128, 255));
		}

		// update the window
		SDL_UpdateWindowSurface(context.window);
	}
} // GAOL::respond()
