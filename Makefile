CPPFLAGS = -Wall -Wpedantic -Wextra -std=c++17 -O2

SDL2_INCLUDE := $(shell pkg-config --cflags-only-I SDL2 SDL2_gfx)
SDL2_LINK := $(shell pkg-config --libs SDL2 SDL2_gfx)

objs/%.o: gaol/%.cpp
	c++ $(CPPFLAGS) $(SDL2_INCLUDE) -c $< -o $@

objs:
	mkdir -p objs

gaol.a: objs objs/gaol.o objs/vec.o
	ar -r gaol.a objs/gaol.o objs/vec.o

%.out: examples/%.cpp gaol.a
	c++ $(CPPFLAGS) $(SDL2_LINK) -Igaol gaol.a $< -o $@
