#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

#define N 100000000

int main(int argc, char* argv[]) {
    int rank, size, i;
    double x, sum = 0.0, pi = 0.0;
    double start, end;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk_size = N / size;
    int chunk_start = rank * chunk_size;
    int chunk_end = (rank + 1) * chunk_size;

    start = MPI_Wtime();
    #pragma omp parallel for private(x) reduction(+:sum)
    for (i = chunk_start; i < chunk_end; i++) {
        x = (i + 0.5) / N;
        sum += 4.0 / (1.0 + x*x);
    }

    double local_pi = sum / N;

    MPI_Reduce(&local_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    end = MPI_Wtime();

    if (rank == 0) {
        printf("Pi es aproximadamente %.16f, calculado en %f segundos.\n", pi, end - start);
    }

    MPI_Finalize();
    return 0;
}
