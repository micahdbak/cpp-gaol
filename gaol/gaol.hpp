#ifndef GAOL_HPP
#define GAOL_HPP

namespace GAOL {
	// Object: represent an object to be displayed on the screen
	class Object {
	public:
		Object(double _x, double _y): x(_x), y(_y) {};
		~Object() {};

		double getX() const { return x; };
		double getY() const { return y; };

		void setX(double _x) { x = _x; };
		void setY(double _y) { y = _y; };
	private:
		double x, y; // current position
	}; // class Object

	// GAOL: create, respond to, and destroy SDL2 window
	class GAOL {
	public:
		GAOL(std::string title);
		~GAOL();

		// push an object into the system
		void push(Object *o) { objects.push_back(o); };

		/* respond: respond to user interaction with window; update
		 * callback: function pointer that will be called every frame;
		 * 	assumed to update data structure pointed to by state
		 * 	with respect to change in time since last frame (delta)
		 * state: data structure to be modified by calling callback */
		void respond(void (*callback)(void *state, double delta), void *state);
	private:
		std::vector<Object*> objects;
	}; // class GAOL
}; // namespace GAOL

#endif
