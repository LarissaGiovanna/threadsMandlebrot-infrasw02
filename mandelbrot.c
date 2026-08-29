#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mandelbrot.h"

double *findC(double beginRealCoord, double beginImagCoord, int width, int height, int i, int j)
{
    double *c = (double *)malloc(2 * sizeof(double)); // malloc array de double com 2 espaços
    if (c == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para o array c.\n");
        exit(1);
    }

    double stepReal = 3.0 / width;  // passo para a coordenada real
    double stepImag = 3.0 / height; // passo para a coordenada imag

    // int matriz[width][height]; // matriz representando o plano cartesiano dividido

    double cReal = beginRealCoord + j * stepReal; // coordenada real do ponto c, onde j e a coluna da matriz
    double cImag = beginImagCoord + i * stepImag; // coordenada imaginária, onde i e a linha da matriz
    printf("%lf\n", beginImagCoord + i * stepImag);

    c[0] = cReal;
    c[1] = cImag;
    printf("c[%d][%d] = %lf + %lfi\n", i, j, c[0], c[1]);
    return c;
}

int isMandelbrot(double *c, double *mandelbrotNumbers, int width, int height, int maxInteractions)
{
    int i;
    double zReal = 0.0; // sempre começa com z = 0 + 0i
    double zImag = 0.0;

    for (i = 0; i < maxInteractions; i++)
    {
        double zRealSquared = zReal * zReal;
        double zImagSquared = zImag * zImag;

        if (zRealSquared + zImagSquared > 4.0)
        {
            return i;
        }

        double tempZReal = zRealSquared - zImagSquared + c[0]; // z_novo_real = z_real² - z_imag² + c_real
        zImag = 2.0 * zReal * zImag + c[1];                    // z_novo_imag = 2 * z_real * z_imag + c_imag
        zReal = tempZReal;                                     // temp foi criado para nao perder a referencia do valor de zReal antes de atualizar zImag
    }
    if (mandelbrotNumbers != NULL)
    {
        mandelbrotNumbers[0] = zReal;
        mandelbrotNumbers[1] = zImag;
    }
    return maxInteractions;
}

int calcIntensity(int numInteractions, int maxInteractions)
{ // verificar isso
    if (numInteractions == maxInteractions)
    {
        return 255; // ponto pertence ao conjunto de Mandelbrot, intensidade mínima
    }
    return (int)(255.0 * numInteractions / maxInteractions);
}

int *mandelbrotSet(double beginRealCoord, double beginImagCoord, int width, int height, int maxInteractions)
{
    // matriz resultados dos conjunto
    double *mandelbrotNumbers = (double *)malloc(2 * sizeof(double)); // aloca memória para armazenar os resultados
    if (mandelbrotNumbers == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para o array mandelbrotNumbers.\n");
        exit(1);
    }

    int *mandelbrotMatrix = (int *)malloc(height * width * sizeof(int)); // matriz para armazenar os resultados do conjunto de Mandelbrot
    if (mandelbrotMatrix == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para o array mandelbrotMatrix.\n");
        exit(1);
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double *c = findC(beginRealCoord, beginImagCoord, width, height, i, j);// c = [a, bi]
            int numInteractions = isMandelbrot(c, mandelbrotNumbers, width, height, maxInteractions); // calcula o número de interações para o ponto c
            free(c);// libera a memória alocada para c

            mandelbrotMatrix[i * width + j] = calcIntensity(numInteractions, maxInteractions); // calcula a intensidade do ponto e armazena na matriz
        }
    }
    free(mandelbrotNumbers); // libera a memória alocada para mandelbrotNumbers
    return mandelbrotMatrix; // retorna a matriz de resultados
}

void printMatrix(int *matrix, int width, int height, FILE *file)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fprintf(file, "%d ", matrix[i * width + j]); // imprime o valor da matriz na posição (i, j)
        }
        fprintf(file, "\n");
    }
}


// Implementação com pthreads
void* calcLines(void *args){
    ThreadArgsMandelbrot *threadArgs = (ThreadArgsMandelbrot *)args;
    double beginRealCoord = threadArgs->beginRealCoord;
    double beginImagCoord = threadArgs->beginImagCoord;
    int width = threadArgs->width;
    int height = threadArgs->height;
    int maxInteractions = threadArgs->maxInteractions;
    int *mandelbrotMatrix = threadArgs->mandelbrotMatrix;
    double *mandelbrotNumbers = threadArgs->mandelbrotNumbers;
    int startLine = threadArgs->startLine;
    int endLine = threadArgs->endLine;

    for(int i = startLine; i < endLine; i++){
        for(int j = 0; j < width; j++){
            double *c = findC(beginRealCoord, beginImagCoord, width, height, i, j);
            int numInteractions = isMandelbrot(c, mandelbrotNumbers, width, height, maxInteractions);
            free(c);
            mandelbrotMatrix[i * width + j] = calcIntensity(numInteractions, maxInteractions);
        }
    }
    return NULL;

}