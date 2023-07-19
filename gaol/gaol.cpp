#include <string>
#include <vector>
#include <cstdint>

#include "gaol.hpp"

#include <cmath>
#include <cassert>
#include <iostream>

extern "C" {
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
}

static struct _context {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *pixels;
} context;

GAOL::Graphic::~Graphic() {
	// do nothing
}

void GAOL::Circle::render(int x, int y) const {
	filledCircleRGBA(context.renderer, x + sx, y + sy, radius,
		color.r, color.g, color.b, color.a);
}

void GAOL::Text::render(int x, int y) const {
	stringRGBA(context.renderer, x + sx, y + sy, text.c_str(),
		color.r, color.g, color.b, color.a);
}

#define HOOK_ANGLE ((1.0/4.0)*M_PI)

void GAOL::Arrow::render(int x, int y) const {
	int dx, dy, dlx, dly, drx, dry;

	dx = (int)(mag * cos(ang));
	dy = (int)(mag * sin(ang));

	dlx = dx - (int)(8.0 * cos(ang + HOOK_ANGLE));
	dly = dy - (int)(8.0 * sin(ang + HOOK_ANGLE));

	drx = dx - (int)(8.0 * cos(ang - HOOK_ANGLE));
	dry = dy - (int)(8.0 * sin(ang - HOOK_ANGLE));

	aalineRGBA(context.renderer, x, y, x + dx, y + dy,
		color.r, color.g, color.b, color.a);
	aalineRGBA(context.renderer, x + dlx, y + dly, x + dx, y + dy,
		color.r, color.g, color.b, color.a);
	aalineRGBA(context.renderer, x + drx, y + dry, x + dx, y + dy,
		color.r, color.g, color.b, color.a);
}

// Object destructor: delete gfx
GAOL::Object::~Object() {
	for (size_t i = 0; i < gfx.size(); i++)
		delete gfx[i]; // delete graphic
}

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
	context.renderer = SDL_CreateRenderer(
		context.window,
		-1,
		SDL_RENDERER_TARGETTEXTURE
	);
	assert(context.renderer != NULL);

	// texture to store pixels
	context.pixels = SDL_CreateTexture(
		context.renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_TARGET,
		w, h
	);
	assert(context.pixels != NULL);

	// default background color is white
	bgcol = Color(255,255,255,255);

	// clear the texture with transparent pixels
	SDL_SetRenderTarget(context.renderer, context.pixels);
	SDL_SetRenderDrawColor(context.renderer, bgcol.r, bgcol.g, bgcol.b, bgcol.a);
	SDL_RenderClear(context.renderer);
	SDL_SetRenderTarget(context.renderer, NULL);

	// window is initialized
} // GAOL::GAOL()

GAOL::GAOL::~GAOL() {
	SDL_DestroyWindow(context.window);
	SDL_DestroyRenderer(context.renderer);
	SDL_DestroyTexture(context.pixels);
	SDL_Quit(); // quit SDL2
}

void GAOL::GAOL::pixel(int x, int y, Color c) {
	SDL_SetRenderTarget(context.renderer, context.pixels);
	SDL_SetRenderDrawColor(context.renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawPoint(context.renderer, x, y);
	SDL_SetRenderTarget(context.renderer, NULL);
}

void GAOL::GAOL::respond(void (*callback)(GAOL *gaol, double delta)) {
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
		callback(this, delta);

		// fill screen with pixels
		SDL_RenderCopy(context.renderer, context.pixels, NULL, NULL);

		// display all objects
		for (size_t i = 0; i < objects.size(); i++)
			objects[i]->render();

		// update the window
		// SDL_UpdateWindowSurface(context.window);
		SDL_RenderPresent(context.renderer);
	}
} // GAOL::respond()
