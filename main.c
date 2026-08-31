#include <stdio.h>
#include <stdlib.h>
#include "mandelbrot.h"
#include <pthread.h>
#include <time.h>

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

    double beginRealCoord = -2;   // coordenada real inicial
    double beginImagCoord = -1.5; // coordenada imaginária inicial

    // ========== IMPLEMENTACAO SERIAL ==========
    double serialTime = 0.0;
    clock_t startSerial = clock();

    int *matrix = mandelbrotSet(beginRealCoord, beginImagCoord, width, height, maxInteractions); // calcula o conjunto de Mandelbrot

    FILE *file = fopen("mandelbrot_lgsc_serial.pgm", "w"); // abre o arquivo para escrita
    if (file == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo para escrita.\n");
        free(matrix); // libera a memória alocada para a matriz antes de sair
        return 1;
    }
    printMatrix(matrix, width, height, file);

    fclose(file); // fecha o arquivo

    clock_t endSerial = clock();
    serialTime = (double)(endSerial - startSerial) / CLOCKS_PER_SEC;
    // ========== FIM IMPLEMENTACAO SERIAL ==========

    // ========== IMPLEMENTACAO PTHREADS1 ==========
    double pthreads1Time = 0.0;
    clock_t startPthreads1 = clock();

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

    // dividir o trabalho entre as threads, no caso cada thread vai calcular uma parte da matriz de resultados do conjunto de Mandelbrot, de acordo com o número de linhas que cada thread deve calcular, ex: se a matriz tem 100 linhas e temos 4 threads, cada thread vai calcular 25 linhas da matriz.
    for (int i = 0; i < numThreads; i++)
    {
        threadArgs[i].threadId = i;
        threadArgs[i].beginRealCoord = beginRealCoord;
        threadArgs[i].beginImagCoord = beginImagCoord;
        threadArgs[i].width = width;
        threadArgs[i].height = height;
        threadArgs[i].maxInteractions = maxInteractions;
        threadArgs[i].mandelbrotMatrix = mandelbrotMatrix;
        threadArgs[i].mandelbrotNumbers = mandelbrotNumbers;
        threadArgs[i].startLine = i * linesPerThread;
        if (i == numThreads - 1)
        {
            threadArgs[i].endLine = height; // última thread pega o resto das linhas
        }
        else
        {
            threadArgs[i].endLine = (i + 1) * linesPerThread;
        }

        pthread_create(&threads[i], NULL, calcLines, &threadArgs[i]);
    }
    // esperar todas as threads terminarem
    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    FILE *filePthreads = fopen("mandelbrot_lgsc_pthreads1.pgm", "w"); // abre o arquivo para escrita
    if (filePthreads == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo para escrita.\n");
        free(mandelbrotMatrix); // libera a memória alocada para a matriz antes de sair
        return 1;
    }
    printMatrix(mandelbrotMatrix, width, height, filePthreads);
    fclose(filePthreads); // fecha o arquivo

    free(mandelbrotNumbers); // libera a memória alocada para mandelbrotNumbers
    free(mandelbrotMatrix); // libera a memória alocada para mandelbrotMatrix

    clock_t endPthreads1 = clock();
    pthreads1Time = (double)(endPthreads1 - startPthreads1) / CLOCKS_PER_SEC;

    // ========== FIM IMPLEMENTACAO PTHREADS1 ==========

    // ========== IMPLEMENTACAO PTHREADS2 ==========
    double pthreads2Time = 0.0;
    clock_t startPthreads2 = clock();

    // matriz resultados dos conjunto
    double *mandelbrotNumbers2 = (double *)malloc(2 * sizeof(double)); // aloca memória para armazenar os resultados
    if (mandelbrotNumbers2 == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para o array mandelbrotNumbers.\n");
        exit(1);
    }

    int *mandelbrotMatrix2 = (int *)malloc(height * width * sizeof(int)); // matriz para armazenar os resultados do conjunto de Mandelbrot
    if (mandelbrotMatrix2 == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para o array mandelbrotMatrix.\n");
        exit(1);
    }

    pthread_t threads2[numThreads];
    ThreadArgsMandelbrot thread2Args[numThreads];

    for (int i = 0; i < numThreads; i++)
    {
        thread2Args[i].threadId = i;
        thread2Args[i].beginRealCoord = beginRealCoord;
        thread2Args[i].beginImagCoord = beginImagCoord;
        thread2Args[i].width = width;
        thread2Args[i].height = height;
        thread2Args[i].maxInteractions = maxInteractions;
        thread2Args[i].mandelbrotMatrix = mandelbrotMatrix2;
        thread2Args[i].mandelbrotNumbers = mandelbrotNumbers2;
        thread2Args[i].numThreads = numThreads;

        pthread_create(&threads2[i], NULL, calcAlternate, &thread2Args[i]);
    }
    // esperar todas as threads terminarem
    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(threads2[i], NULL);
    }

    FILE *filePthreads2 = fopen("mandelbrot_lgsc_pthreads2.pgm", "w"); // abre o arquivo para escrita
    if (filePthreads2 == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo para escrita.\n");
        free(mandelbrotMatrix2); // libera a memória alocada para a matriz antes de sair
        return 1;
    }
    printMatrix(mandelbrotMatrix2, width, height, filePthreads2);
    fclose(filePthreads2); // fecha o arquivo

    free(mandelbrotNumbers2); // libera a memória alocada para mandelbrotNumbers
    free(mandelbrotMatrix2); // libera a memória alocada para mandelbrotMatrix

    clock_t endPthreads2 = clock();
    pthreads2Time = (double)(endPthreads2 - startPthreads2) / CLOCKS_PER_SEC;
    // ========== FIM IMPLEMENTACAO PTHREADS2 ==========

    // ========== IMPLEMENTACAO OPENMP ==========
    double openmpTime = 0.0;
    clock_t startOpenMP = clock();

    int *mandelbrotMatrixOpenMP = mandelbrotSetOpenMP(beginRealCoord, beginImagCoord, width, height, maxInteractions, numThreads); // calcula o conjunto de Mandelbrot usando OpenMP
    FILE *fileOpenMP = fopen("mandelbrot_lgsc_openmp.pgm", "w");                                                                   // abre o arquivo para escrita
    if (fileOpenMP == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo para escrita.\n");
        free(mandelbrotMatrixOpenMP); // libera a memória alocada para a matriz antes de sair
        return 1;
    }
    printMatrix(mandelbrotMatrixOpenMP, width, height, fileOpenMP);
    fclose(fileOpenMP); // fecha o arquivo

    free(mandelbrotMatrixOpenMP); // libera a memória alocada para a matriz
    clock_t endOpenMP = clock();
    openmpTime = (double)(endOpenMP - startOpenMP) / CLOCKS_PER_SEC;
    // ========== FIM IMPLEMENTACAO OPENMP ==========

    free(matrix); // libera a memória alocada para a matriz

    // ========== PRINT TEMPOS DE EXECUCAO ==========
    FILE *timeFile = fopen("times.txt", "w");
    if (timeFile == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo para escrita.\n");
        return 1;
    }
    fprintf(timeFile, "Serial: %fs\n", serialTime);
    fprintf(timeFile, "OpenMP: %fs\n", openmpTime);
    fprintf(timeFile, "Pthreads1: %fs\n", pthreads1Time);
    fprintf(timeFile, "Pthreads2: %fs\n", pthreads2Time);
    fclose(timeFile);

    return 0;
}