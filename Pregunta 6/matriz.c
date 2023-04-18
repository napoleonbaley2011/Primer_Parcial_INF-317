#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define N 100 // tamaño de la matriz

int main(int argc, char **argv)
{
    int size, rank;
    int i, j, k;
    int a[N][N], b[N][N], c[N][N];

    MPI_Init(&argc, &argv); // inicialización de MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size); // obtiene el tamaño del comunicador global
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // obtiene el rango del proceso actual en el comunicador global

    if (rank == 0) { // solo el proceso 0 inicializa la matriz 'a' y la matriz 'b'
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                a[i][j] = i + j;
                b[i][j] = i - j;
            }
        }
    }

    // se reparte la matriz 'b' entre todos los procesos mediante la función MPI_Bcast
    MPI_Bcast(b, N*N, MPI_INT, 0, MPI_COMM_WORLD);

    // se divide el trabajo en múltiples hilos usando OpenMP
    #pragma omp parallel shared(a, b, c) private(i, j, k)
    {
        #pragma omp for // divide el ciclo for en múltiples hilos
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                c[i][j] = 0; // inicializa la matriz 'c' en 0
                for (k = 0; k < N; k++) {
                    c[i][j] += a[i][k] * b[k][j]; // realiza la multiplicación de matrices
                }
            }
        }
    }

    // se suman los resultados parciales de cada proceso usando MPI_Reduce
    MPI_Reduce(rank == 0 ? MPI_IN_PLACE : c, c, N*N, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) { // solo el proceso 0 imprime la matriz resultante 'c'
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%d ", c[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize(); // finalización de MPI
    return 0;
}
