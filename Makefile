CPPFLAGS = -Wall -Wpedantic -Wextra -std=c++17 -O2

objs/%.o: gaol/%.cpp
	c++ $(CPPFLAGS) -c $< -o $@

objs:
	mkdir -p objs

.PHONY: gaol
gaol: objs objs/gaol.o
	ar -r gaol.a objs/gaol.o

%.out: examples/%.cpp gaol
	c++ $(CPPFLAGS) -Igaol gaol.a $< -o $@
