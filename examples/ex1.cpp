#include <string>
#include <vector>
#include <cstdint>
#include <cmath>

#include <iostream>

#include "gaol.hpp"

#define PRADIUS   4
#define METRESPP  0.001 // metres per pixel
#define SLOW      0.01 // 1s = Xs

class Particle {
public:
	Particle(
		double x, double y,
		double vx, double vy,
		double _charge, double _mass,
		std::string _name,
		GAOL::Color _color)
	{
		s.set(x, y);
		v.set(vx, vy);
		charge = _charge;
		mass = _mass;
		name = _name;
		color = _color;

		o = new GAOL::Object;
		o->add_graphic(new GAOL::Arrow(0, 0, PURPLE, 0.0, 48.0)); // velocity
		o->add_graphic(new GAOL::Arrow(0, 0, ORANGE, M_PI / 2.0, 32.0)); // acceleration
		o->add_graphic(new GAOL::Text(-16, 16, BLACK, _name));
		o->add_graphic(new GAOL::Circle(0, 0, _color, PRADIUS));
	}

	~Particle() { delete o; };

	GAOL::Object *o;
	GAOL::vec s, v;
	double charge, mass;
	std::string name;
	GAOL::Color color;
};

struct State {
	std::vector<Particle *> particles;
} state;

#define K    8987551787.0    // Coulomb's Constant
#define EC   1.60217646e-19  // Elementary Charge (C)
#define AMU  1.66053873e-27  // Atomic Mass Unit (kg)
#define PM   1.67262158e-27  // Proton Mass (kg)
#define NM   1.67492716e-27  // Neutron Mass (kg)
#define EM   9.10938188e-31  // Electron Mass (kg)

void callback(GAOL::GAOL *gaol, double delta) {
	for (size_t i = 0; i < state.particles.size(); i++) {
		Particle &q1 = *state.particles[i];
		GAOL::vec felec;

		for (size_t j = 0; j < state.particles.size(); j++) {
			if (i == j)
				continue;

			Particle &q2 = *state.particles[j];

			// electromagnetic force between these two particles
			GAOL::vec f = q1.s - q2.s;
			f *= METRESPP; // multiply the distance by metres per pixel

			double magnitude = f.mag();
			f /= magnitude; // f.mag() = 1.0

			// let magnitude be the distance between these two particles squared
			magnitude *= magnitude;

			// f.mag() = q2.charge / magnitude
			f *= q2.charge / magnitude;

			felec += f;
		}

		felec *= K * q1.charge;

		GAOL::vec acc = felec / q1.mass;

		std::cout << q1.name << ": " << std::endl
			<< "\tacc: " << acc.x << ", " << acc.y << std::endl
			<< "\tvel: " << q1.v.x << ", " << q1.v.y << std::endl;

		q1.v += acc * delta * SLOW;
		q1.s += q1.v * delta * SLOW;
		q1.o->move(q1.s);
		gaol->pixel((int)q1.s.x, (int)q1.s.y, q1.color + 64);

		// set arrow
		GAOL::Arrow *varr = (GAOL::Arrow *)q1.o->graphic(0);
		GAOL::Arrow *aarr = (GAOL::Arrow *)q1.o->graphic(1);

		varr->ang = q1.v.ang();
		varr->mag = fmin(q1.v.mag() + 2*PRADIUS, 128.0);
		aarr->ang = acc.ang();
		aarr->mag = fmin(acc.mag() + 2*PRADIUS, 96.0);
	}
}

int main() {
	// create SDL2 window
	GAOL::GAOL gaol("Example 1", 32, 32, 960, 960);
	gaol.bgcol = WHITE;

	Particle
	proton1(
		480.0, 480.0,
		0.0, 0.0,
		3.0 * EC, 3 * PM + 4 * NM,
		"Nucleus", BLUE),
	electron1(
		480 - 32.0, 480.0,
		0.0, 4024.0,
		-1.0 * EC, EM,
		"Electron 1", ORANGE),
	electron2(
		480.0, 480.0 - 320.0,
		-1200.0, 0.0,
		-1.0 * EC, EM,
		"Electron 2", PINK),
	electron3(
		480.0, 480.0 + 320.0,
		1200.0, 0.0,
		-1.0 * EC, EM,
		"Electron 3", MAGENTA);

	state.particles.push_back(&proton1);
	state.particles.push_back(&electron1);
	state.particles.push_back(&electron2);
	state.particles.push_back(&electron3);

	gaol.push(proton1.o);
	gaol.push(electron1.o);
	gaol.push(electron2.o);
	gaol.push(electron3.o);

	// update state every frame
	gaol.respond(callback);
	// gaol cleans up SDL2 as it is destroyed
}
