#include <string>
#include <vector>

#include "gaol.hpp"

#include <iostream>
#include <cassert>

extern "C" {
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
}

static struct _context {
	SDL_Window *window;
	SDL_Surface *screen;
	SDL_Renderer *renderer;
} context;

GAOL::Graphic::~Graphic() {
	// do nothing
}

void GAOL::Circle::render(int x, int y) const {
	filledCircleColor(context.renderer, x + sx, y + sy, radius, color);
} // void Circle::render(...)

void GAOL::Text::render(int x, int y) const {
	stringColor(context.renderer, x + sx, y + sy, text.c_str(), WHITE);
} // void Text::render(...)

// Object destructor: delete gfx
GAOL::Object::~Object() {
	for (size_t i = 0; i < gfx.size(); i++) {
		delete gfx[i]; // delete graphic
	}
} // Object::~Object()

GAOL::GAOL::GAOL(std::string title, int x, int y, int w, int h, Color _bgcol) {
	// initialize SDL2
	assert(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) == 0);

	// create window
	context.window = SDL_CreateWindow(
		title.c_str(), // title of window
		x, y, // x, y positions
		w, h, // width, height
		0 // default flags
	);
	assert(context.window != NULL);

	// get surface of window
	//context.screen = SDL_GetWindowSurface(context.window);
	//assert(context.screen != NULL);

	// set background color
	bgcol = _bgcol;

	// create renderer for window
	context.renderer = SDL_CreateRenderer(context.window, -1, 0);

	// window is initialized
} // GAOL::GAOL()

GAOL::GAOL::~GAOL() {
	SDL_DestroyRenderer(context.renderer);
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

		// fill screen with pixels of the background color
		//SDL_FillRect(context.screen, NULL, 0);
		SDL_SetRenderDrawColor(context.renderer,
			(bgcol >> 24) & 0xff, // red
			(bgcol >> 16) & 0xff, // green
			(bgcol >> 8) & 0xff, // blue
			bgcol & 0xff); // alpha
		SDL_RenderClear(context.renderer);

		// display all objects
		for (size_t i = 0; i < objects.size(); i++)
			objects[i]->render();

		// update the window
		// SDL_UpdateWindowSurface(context.window);
		SDL_RenderPresent(context.renderer);
	}
} // GAOL::respond()
