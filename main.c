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

    //dividir o trabalho entre as threads, no caso cada thread vai calcular uma parte da matriz de resultados do conjunto de Mandelbrot, de acordo com o número de linhas que cada thread deve calcular, ex: se a matriz tem 100 linhas e temos 4 threads, cada thread vai calcular 25 linhas da matriz.
    for (int i = 0; i < numThreads; i++){
        threadArgs[i].threadId = i;
        threadArgs[i].beginRealCoord = beginRealCoord;
        threadArgs[i].beginImagCoord = beginImagCoord;
        threadArgs[i].width = width;
        threadArgs[i].height = height;
        threadArgs[i].maxInteractions = maxInteractions;
        threadArgs[i].mandelbrotMatrix = mandelbrotMatrix;
        threadArgs[i].mandelbrotNumbers = mandelbrotNumbers;
        threadArgs[i].startLine = i * linesPerThread;
        if (i == numThreads - 1) {
            threadArgs[i].endLine = height; // última thread pega o resto das linhas
        } else {
            threadArgs[i].endLine = (i + 1) * linesPerThread;
        }

        pthread_create(&threads[i], NULL, calcLines, &threadArgs[i]);
    }
    //esperar todas as threads terminarem
    for (int i = 0; i < numThreads; i++){
        pthread_join(threads[i], NULL);
    }

    FILE *filePthreads = fopen("mandelbrot_lgsc_pthreads.pgm", "w"); // abre o arquivo para escrita
    if (filePthreads == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo para escrita.\n");
        free(mandelbrotMatrix); // libera a memória alocada para a matriz antes de sair
        return 1;
    }
    printMatrix(mandelbrotMatrix, width, height, filePthreads);
    fclose(filePthreads); // fecha o arquivo




    
    free(mandelbrotNumbers); // libera a memória alocada para mandelbrotNumbers

    // ========== FIM IMPLEMENTACAO PTHREADS1 ==========


    free(matrix); // libera a memória alocada para a matriz


    return 0;
}