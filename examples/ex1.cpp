#include <iostream>

#include "gaol.hpp"

int main() {
	GAOL::GAOL gaol("Example 1");

	gaol.respond(); // respond to user interaction
	// gaol cleans up as it is destroyed
}
