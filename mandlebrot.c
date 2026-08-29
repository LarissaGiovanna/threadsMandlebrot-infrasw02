# include <stdio.h>
# include <stdlib.h>

int *findC(int beginRealCoord, int beginImagCoord, int width, int height, int i, int j){
    int *c = malloc(2 * sizeof(int)); //malloc array de inteiros com 2 espaços
    if (c == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o array c.\n");
        exit(1);
    }

    double stepReal = 3.0 / width; // passo para a coordenada real
    double stepImag = 3.0 / height; // passo para a coordenada imag

    //int matriz[width][height]; // matriz representando o plano cartesiano dividido

    double cReal = beginRealCoord + j * stepReal; // coordenada real do ponto c, onde j e a coluna da matriz
    double cImag = beginImagCoord + i * stepImag; // coordenada imaginária, onde i e a linha da matriz

    c[0] = cReal;
    c[1] = cImag;
    return c;
}

int isMandelbrot(int z, int *c, double *mandelbrotNumbers, int width, int height, int maxInteractions) {
    int i;
    double zReal = 0.0;
    double zImag = 0.0;

    for (i = 0; i < maxInteractions; i++) {
        double zRealSquared = zReal * zReal;
        double zImagSquared = zImag * zImag;

        if (zRealSquared + zImagSquared > 4.0) {
            return i;
        }

        double tempZReal = zRealSquared - zImagSquared + c[0]; //z_novo_real = z_real² - z_imag² + c_real
        zImag = 2.0 * zReal * zImag + c[1]; //z_novo_imag = 2 * z_real * z_imag + c_imag
        zReal = tempZReal; //temp foi criado para nao perder a referencia do valor de zReal antes de atualizar zImag
    }
    if (mandelbrotNumbers != NULL){
        mandelbrotNumbers[0] = zReal;
        mandelbrotNumbers[1] = zImag;
    }
    return maxInteractions;
}



int main (int argc, char *argv[]) {
    int width = atof(argv[1]);
    int height = atof(argv[2]);
    int maxInteractions = atoi(argv[3]);
    int numThreads = atoi(argv[4]);
    if (argc != 5) {
        printf("Uso de parametros corretos: %s <largura> <altura> <maxInteracoes> <numThreads>\n", argv[0]);
        return 1;
    }
    printf("Width: %d, Height: %d, Max Interactions: %d, Num Threads: %d\n", width, height, maxInteractions, numThreads);

    return 0;
}