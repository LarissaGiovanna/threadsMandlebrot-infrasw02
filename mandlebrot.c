# include <stdio.h>
# include <stdlib.h>

int isMandelbrot(int z, int c){
    int i;
    for (i = 0; i < 100; i++) { //100 = num interacoes para ver se a conta realizada se repete muito
        if (z * z + c > 4) {
            return i;
        }
        z = z * z + c;
    }
    return 100;
}

int main (int argc, char *argv[]) {
    char *width = argv[1];
    char *height = argv[2];
    int maxInteractions = atoi(argv[3]);
    int numThreads = atoi(argv[4]);
    if (argc != 5) {
        printf("Uso de parametros corretos: %s <largura> <altura> <maxInteracoes> <numThreads>\n", argv[0]);
        return 1;
    }
    printf("Width: %s, Height: %s, Max Interactions: %d, Num Threads: %d\n", width, height, maxInteractions, numThreads);
    return 0;
}