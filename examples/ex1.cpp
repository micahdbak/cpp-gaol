#include <string>
#include <vector>

#include "gaol.hpp"

#include <iostream>

struct State {
	GAOL::Object *box;
	double x, y;
};

#define PPS  32

void callback(void *_state, double delta) {
	State &state = *(State *)_state;

	state.x += PPS * delta;

	if (state.x > 256.0)
		state.x = 64.0;

	state.box->x = (int)state.x;
	state.box->y = (int)state.y;

	//std::cout << "Time since last frame: " << delta << std::endl;
}

int main() {
	// create SDL2 window
	GAOL::GAOL gaol("Example 1", 32, 32, 512, 512, BLACK);

	// prepare application state
	State state;

	state.x = state.y = 64.0;
	state.box = new GAOL::Object;
	state.box->add_graphic(new GAOL::Circle(0, 0, RED, 16));
	state.box->add_graphic(new GAOL::Text(-16, 16, "Hello, world!"));

	gaol.push(state.box); // let gaol know about this object

	// update state every frame
	gaol.respond(callback, &state);
	// gaol cleans up SDL2 as it is destroyed

	delete state.box;
}
