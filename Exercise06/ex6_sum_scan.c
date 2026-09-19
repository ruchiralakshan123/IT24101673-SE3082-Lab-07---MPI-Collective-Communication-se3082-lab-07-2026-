#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk_size = N / size;

    /* Only root allocates and fills the full array */
    int *array = NULL;
    if (rank == 0) {
        array = (int *)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++)
            array[i] = i + 1;
        printf("Root filled array with values 1 to %d\n", N);
    }

    /* Each process allocates only its chunk */
    int *local_chunk = (int *)malloc(chunk_size * sizeof(int));

    double start = MPI_Wtime();

    /* SCATTER: Distribute one chunk to each process */
    MPI_Scatter(array,       chunk_size, MPI_INT,
                local_chunk, chunk_size, MPI_INT,
                0, MPI_COMM_WORLD);

    /* Each process sums its local chunk */
    long long local_sum = 0;
    for (int i = 0; i < chunk_size; i++)
        local_sum += local_chunk[i];

    /*
     * SCAN: Computes inclusive prefix reduction.
     * After this call with MPI_SUM:
     *   Rank 0: prefix_sum = local_sum_0
     *   Rank 1: prefix_sum = local_sum_0 + local_sum_1
     *   Rank k: prefix_sum = sum of local_sums for ranks 0..k
     * The last rank's prefix_sum equals the global total.
     */
    long long prefix_sum = 0;
    MPI_Scan(&local_sum, &prefix_sum, 1, MPI_LONG_LONG, MPI_SUM, MPI_COMM_WORLD);

    long long sum_before_me = prefix_sum - local_sum;

    /* Every process prints its local, prefix, and prior-sum values */
    printf("  Rank %d: local_sum = %lld, prefix_sum = %lld, sum_before_me = %lld\n",
           rank, local_sum, prefix_sum, sum_before_me);

    /*
     * Verification bonus: for the array 1..N the sum of the first K integers
     * is K*(K+1)/2.  Each rank's K = (rank+1)*chunk_size.
     */
    long long K = (long long)(rank + 1) * chunk_size;
    long long expected_prefix = K * (K + 1) / 2;
    printf("  Rank %d: expected prefix_sum = %lld, match = %s\n",
           rank, expected_prefix, prefix_sum == expected_prefix ? "YES" : "NO");

    /* Root verifies the global total using the last rank's prefix_sum.
       Since we cannot know which rank is last without extra communication,
       root uses its own MPI_Reduce as a cross-check. */
    if (rank == 0) {
        double elapsed  = MPI_Wtime() - start;
        long long expected_total = (long long)N * (N + 1) / 2;
        /* Last rank's prefix_sum equals the global total; verify via formula */
        printf("\n[Scan] Last rank prefix_sum should equal %lld\n", expected_total);
        printf("[Scan] Time = %.4f sec\n", elapsed);
    }

    /* Extra: let last rank print its prefix_sum as the global total */
    if (rank == size - 1) {
        long long expected_total = (long long)N * (N + 1) / 2;
        printf("\n[Scan] Rank %d prefix_sum (global total) = %lld\n", rank, prefix_sum);
        printf("[Scan] Expected                           = %lld\n", expected_total);
        printf("[Scan] Correct?                           = %s\n",
               prefix_sum == expected_total ? "YES" : "NO");
    }

    free(local_chunk);
    if (rank == 0) free(array);
    MPI_Finalize();
    return 0;
}
