#include <stdio.h>
#include <stdlib.h>
#include "mandelbrot.h"

#include <pthread.h>

int main(int argc, char *argv[])
{
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int maxInteractions = atoi(argv[3]);
    int numThreads = atoi(argv[4]);
    if (argc != 5)
    {
        printf("Uso de parametros corretos: %s <largura> <altura> <maxInteracoes> <numThreads>\n", argv[0]);
        return 1;
    }
    printf("Width: %d, Height: %d, Max Interactions: %d, Num Threads: %d\n", width, height, maxInteractions, numThreads);

    double beginRealCoord = -2;   // coordenada real inicial
    double beginImagCoord = -1.5; // coordenada imaginária inicial

    int *matrix = mandelbrotSet(beginRealCoord, beginImagCoord, width, height, maxInteractions); // calcula o conjunto de Mandelbrot
    
    // ========== IMPLEMENTACAO SERIAL ==========
    FILE *file = fopen("mandelbrot_lgsc_serial.pgm", "w"); // abre o arquivo para escrita
    if (file == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo para escrita.\n");
        free(matrix); // libera a memória alocada para a matriz antes de sair
        return 1;
    }
    printMatrix(matrix, width, height, file);

    printf("saida no arquivo .pgm\n");
    fclose(file); // fecha o arquivo
    // ========== FIM IMPLEMENTACAO SERIAL ==========

    // ========== IMPLEMENTACAO PTHREADS1 ==========
    
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

    pthread_t threads[numThreads];
    ThreadArgsMandelbrot threadArgs[numThreads];
    int linesPerThread = height / numThreads;

    









    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
           
        }
    }
    free(mandelbrotNumbers); // libera a memória alocada para mandelbrotNumbers

    // ========== FIM IMPLEMENTACAO PTHREADS1 ==========


    free(matrix); // libera a memória alocada para a matriz


    return 0;
}