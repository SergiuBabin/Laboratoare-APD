#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MULTI 5 // chunk dimension
#define ROOT 0

int main (int argc, char *argv[])
{
    int  numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    int num_elements = MULTI * numtasks; // total elements
    int *v_send = NULL; // full vector
    int *v_recv = (int *)malloc(MULTI * sizeof(int)); // partial vector

    // ROOT process generates the values for the full vector.
    if (rank == ROOT) {
        v_send = malloc(MULTI * numtasks * sizeof(int) );
        for (int i = 0; i < MULTI * numtasks; i++)
            v_send[i] = 0;

        printf("[%d] has data: ", rank);
        for (int i = 0; i < 5 * numtasks; i++)
            printf("%d ", v_send[i]);
        printf("\n");
    }
    // Scatter the vector to all processes.
    MPI_Scatter(v_send, MULTI, MPI_INT, v_recv, MULTI, MPI_INT, 0, MPI_COMM_WORLD);
    /*
     * Prints the values received after scatter.
     * NOTE: If MULTI changed, also change this line.
     */

    for (int i = 0; i < MULTI; i++) {
        printf("Before: rank [%d] - value = %d\n", rank, v_recv[i]);
        v_recv[i] += rank;
        printf("After: rank [%d] - value = %d\n", rank, v_recv[i]);
    }

    // Each process increments the values of the partial vector received.
    // Gathers the values from all the processes.
    MPI_Gather(v_recv, MULTI, MPI_INT, v_send, MULTI, MPI_INT, 0, MPI_COMM_WORLD);
    // The ROOT process prints the elements received.
    if (rank == ROOT) {

        printf("[%d] has data: ", rank);
        for (int i = 0; i < MULTI * numtasks; i++)
            printf("%d ", v_send[i]);
        printf("\n");

        free(v_send);
        free(v_recv);
    }
    MPI_Finalize();

}
