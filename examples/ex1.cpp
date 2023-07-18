#include <string>
#include <vector>

#include "gaol.hpp"

#include <iostream>

struct State {
	GAOL::Object *box;
};

#define PPS  32

void callback(void *_state, double delta) {
	State *state = (State *)_state;

	double x = state->box->getX() + PPS * delta;

	if (x > 256.0)
		x = 64.0;

	state->box->setX(x);

	std::cout << "Time since last frame: " << delta << std::endl;
}

int main() {
	// create SDL2 window
	GAOL::GAOL gaol("Example 1");

	// prepare application state
	State state;
	state.box = new GAOL::Object(64.0, 64.0); // make box at (64.0,64.0)
	gaol.push(state.box); // let gaol know about this object

	// update state every frame
	gaol.respond(callback, &state);
	// gaol cleans up SDL2 as it is destroyed

	delete state.box;
}
