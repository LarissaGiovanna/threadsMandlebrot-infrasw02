#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

double *findC(double beginRealCoord, double beginImagCoord, int width, int height, int i, int j);
int isMandelbrot(double *c, double *mandelbrotNumbers, int width, int height, int maxInteractions);
int calcIntensity(int numInteractions, int maxInteractions);
int *mandelbrotSet(double beginRealCoord, double beginImagCoord, int width, int height, int maxInteractions);
void printMatrix(int *matrix, int width, int height, FILE *file);

//pthreads
typedef struct threadArgsMandelbrot{
        double beginRealCoord;
        double beginImagCoord;
        int width;
        int height;
        int maxInteractions;
        double *mandelbrotNumbers;
        int *mandelbrotMatrix;

        int startLine;
        int endLine;
    } ThreadArgsMandelbrot;

void *mandelbrotSetPthreads(void *args);