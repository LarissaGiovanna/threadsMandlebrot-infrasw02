# include <stdio.h>
# include <stdlib.h>

int main (int argc, char *argv[]) {
    char *width = argv[1];
    char *height = argv[2];
    int maxInteractions = atoi(argv[3]);
    int numThreads = atoi(argv[4]);
    printf("Width: %s, Height: %s, Max Interactions: %d, Num Threads: %d\n", width, height, maxInteractions, numThreads);
    return 0;
}