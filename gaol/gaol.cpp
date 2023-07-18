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
	SDL_Renderer *renderer;
} context;

// true if little endian; false if big endian
bool endianness() {
	uint16_t test = 0x0001;
	char *order = (char *)&test;
	return order[0];
}

GAOL::Color GAOL::RGBA(uint32_t r, uint32_t g, uint32_t b, uint32_t a) {
	Color color;

	// little endian
	if (endianness())
		color = (a << 24) | (b << 16) | (g << 8) | r;
	else
		color = (r << 24) | (g << 16) | (b << 8) | a;

	return color;
}

GAOL::Graphic::~Graphic() {
	// do nothing
}

void GAOL::Circle::render(int x, int y) const {
	filledCircleColor(context.renderer, x + sx, y + sy, radius, color);
} // void Circle::render(...)

void GAOL::Text::render(int x, int y) const {
	stringColor(context.renderer, x + sx, y + sy, text.c_str(), color);
} // void Text::render(...)

// Object destructor: delete gfx
GAOL::Object::~Object() {
	for (size_t i = 0; i < gfx.size(); i++)
		delete gfx[i]; // delete graphic
} // Object::~Object()

GAOL::GAOL::GAOL(std::string title, int x, int y, int w, int h) {
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

	// create renderer for window
	context.renderer = SDL_CreateRenderer(context.window, -1, 0);

	// default background color is white
	bgcol = RGBA(255, 255, 255, 255);

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

	uint8_t bgr = (bgcol >> 24) & 0xff;
	uint8_t bgg = (bgcol >> 16) & 0xff;
	uint8_t bgb = (bgcol >> 8) & 0xff;

	if (endianness()) {
		bgr = bgcol & 0xff;
		bgg = (bgcol >> 8) & 0xff;
		bgb = (bgcol >> 16) & 0xff;
	}

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
		SDL_SetRenderDrawColor(context.renderer, bgr, bgg, bgb, 0xff);
		SDL_RenderClear(context.renderer);

		// display all objects
		for (size_t i = 0; i < objects.size(); i++)
			objects[i]->render();

		// update the window
		// SDL_UpdateWindowSurface(context.window);
		SDL_RenderPresent(context.renderer);
	}
} // GAOL::respond()
