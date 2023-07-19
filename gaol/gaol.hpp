#ifndef GAOL_HPP
#define GAOL_HPP

#define BLACK   GAOL::Color(0,0,0,255)
#define RED     GAOL::Color(255,0,0,255)
#define PINK    GAOL::Color(255,128,128,255)
#define ORANGE  GAOL::Color(255,128,0,255)
#define YELLOW  GAOL::Color(255,255,0,255)
#define GREEN   GAOL::Color(0,255,0,255)
#define TEAL    GAOL::Color(0,255,255,255)
#define BLUE    GAOL::Color(0,0,255,255)
#define PURPLE  GAOL::Color(255,0,255,255)
#define MAGENTA GAOL::Color(255,128,255,255)
#define GRAY    GAOL::Color(128,128,128,255)
#define WHITE   GAOL::Color(255,255,255,255)

namespace GAOL {
	class Color {
	public:
		Color(): r(0), g(0), b(0), a(0) {}
		Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a):
			r(_r), g(_g), b(_b), a(_a) {}

		int r, g, b, a;

		Color operator+(int amnt) const {
			Color c(*this);

			c.r += amnt;
			c.r = c.r > 255 ? 255 : c.r;
			c.g += amnt;
			c.g = c.g > 255 ? 255 : c.g;
			c.b += amnt;
			c.b = c.b > 255 ? 255 : c.b;

			return c;
		}

		Color operator-(int amnt) const {
			Color c(*this);

			c.r -= amnt;
			c.r = c.r < 0 ? 0 : c.r;
			c.g -= amnt;
			c.g = c.g < 0 ? 0 : c.g;
			c.b -= amnt;
			c.b = c.b < 0 ? 0 : c.b;

			return c;
		}
	}; // Color

	class vec {
	public:
		vec(): x(0.0), y(0.0) {}
		vec(double _x, double _y): x(_x), y(_y) {}
		~vec() {}

		double x, y;

		void set(double _x, double _y);
		void set_ang(double ang, double mag);
		double mag() const;
		double ang() const;

		vec operator=(vec other);
		vec operator+(vec other) const;
		vec operator+=(vec other);
		vec operator-(vec other) const;
		vec operator-=(vec other);
		vec operator*(vec other) const;
		vec operator*=(vec other);
		vec operator*(double scalar) const;
		vec operator*=(double scalar);
		vec operator/(vec other) const;
		vec operator/=(vec other);
		vec operator/(double scalar) const;
		vec operator/=(double scalar);
	}; // class vec

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

		int sx, sy;
		Color color;
		int radius;

		virtual void render(int x, int y) const;
	}; // class Circle

	class Text: public Graphic {
	public:
		Text() {}
		Text(int _sx, int _sy, Color _color, std::string _text):
			sx(_sx), sy(_sy), color(_color), text(_text) {}

		~Text() {}

		int sx, sy;
		Color color;
		std::string text;

		virtual void render(int x, int y) const;
	}; // class Text

	class Arrow: public Graphic {
	public:
		Arrow() {}
		Arrow(int _sx, int _sy, Color _color, double _ang, double _mag):
			sx(_sx), sy(_sy), color(_color), ang(_ang), mag(_mag) {}

		~Arrow() {}

		int sx, sy;
		Color color;
		double ang, mag;

		virtual void render(int x, int y) const;
	}; // class Arrow

	// Object: represent an object to be displayed on the screen
	class Object {
	public:
		Object(): x(0), y(0) {};
		Object(int _x, int _y): x(_x), y(_y) {};

		~Object(); // will delete all gfx

		int x, y;

		void move(vec &v) {
			x = (int)v.x;
			y = (int)v.y;
		}

		void add_graphic(Graphic *graphic) {
			gfx.push_back(graphic);
		}

		void render() const {
			// for every graphic
			for (size_t i = 0; i < gfx.size(); i++)
				gfx[i]->render(x, y); // render graphic
		}

		Graphic *graphic(int i) {
			return gfx[i];
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
		GAOL(std::string title, int x, int y, int w, int h);
		~GAOL();

		Color bgcol; // background color

		// push an object into the system
		void push(Object *o) { objects.push_back(o); };

		// add a pixel onto the screen with the specified coordinates and color
		void pixel(int x, int y, Color color);

		/* respond: respond to user interaction with window; update
		 * callback: function pointer that will be called every frame;
		 * 	assumed to update an external data structure
		 * 	with respect to change in time since last frame (delta) */
		void respond(void (*callback)(GAOL *gaol, double delta));
	private:
		std::vector<Object*> objects;
	}; // class GAOL
}; // namespace GAOL

#endif
