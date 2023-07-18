# GAOL

C++ (G)raphics (a)nd (O)bjects (L)ibrary utilizing the SDL2 windowing library as a base.

## Usage as a library

To use GAOL as a library, please create the `gaol.a` archive file with:

```
make gaol.a
```

And compile your program with:

```
c++ ... -Igaol gaol.a ...
```

The `-Igaol` flag defines the path to the `gaol` folder as found in this repository.
If you are utilizing this library from a different directory, the `-I` flag must
specify the path to the `gaol` directory relative to your immediate directory.
Additionally, SDL2 must be properly linked with `gaol.a`.

## Examples

To build any of the provided examples, please execute the following rule, with `%` representing
the name of the example file as found in the `examples` directory (excluding the `.cpp` file extension):

```
make %.out
```
