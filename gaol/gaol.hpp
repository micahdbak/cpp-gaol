#ifndef GAOL_HPP
#define GAOL_HPP

#include <cstdint> // for uint32_t (Color)

#define BLACK   0x000000ff
#define RED     0xff0000ff
#define YELLOW  0xffff00ff
#define GREEN   0x00ff00ff
#define TEAL    0x00ffffff
#define BLUE    0x0000ffff
#define PURPLE  0xff00ffff
#define GRAY    0x808080ff
#define WHITE   0xffffffff

namespace GAOL {
	typedef uint32_t Color; // Color

	class Graphic {
	public:
		Graphic() {}
		virtual ~Graphic();

		// render function is undefined by default
		virtual void render(int x, int y) const = 0;
	}; // class Graphic

	class Circle: public Graphic {
	public:
		Circle() {}
		Circle(int _sx, int _sy, Color _color, int _radius):
			sx(_sx), sy(_sy), color(_color), radius(_radius) {}

		~Circle() {}

		int sx, sy; // shift x, shift y
		Color color;
		int radius;

		virtual void render(int x, int y) const;
	}; // class Circle

	class Text: public Graphic {
	public:
		Text() {}
		Text(int _sx, int _sy, std::string _text):
			sx(_sx), sy(_sy), text(_text) {}

		~Text() {}

		int sx, sy; // shift x, shift y
		std::string text;

		virtual void render(int x, int y) const;
	}; // class Text

	// Object: represent an object to be displayed on the screen
	class Object {
	public:
		Object(): x(0), y(0) {};
		Object(int _x, int _y): x(_x), y(_y) {};

		~Object(); // will delete all gfx

		int x, y;

		void add_graphic(Graphic *graphic) {
			gfx.push_back(graphic);
		}

		void render() const {
			// for every graphic
			for (size_t i = 0; i < gfx.size(); i++)
				gfx[i]->render(x, y); // render graphic
		}
	private:
		std::vector<Graphic *> gfx;
	}; // class Object

	// GAOL: create, respond to, and destroy SDL2 window
	class GAOL {
	public:
		// GAOL constructor;
		// title: window title
		// x: x-position of window
		// y: y-position of window
		// w: width of window
		// h: height of window
		GAOL(std::string title, int x, int y, int w, int h, Color _bgcol);
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
		Color bgcol; // background color
		std::vector<Object*> objects;
	}; // class GAOL
}; // namespace GAOL

#endif
