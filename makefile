mandelbrot: main.c mandelbrot.c mandelbrot.h
	gcc -o mandelbrot main.c mandelbrot.c -I.

clean:
	rm -rf mandelbrot *.o *.pgm