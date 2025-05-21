#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void print_matrix(int *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%2d ", matrix[i * n + j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    int rank, size, n = 4; // Ejemplo con matriz 4x4
    int *matrix = NULL;
    int *triangular = NULL;
    MPI_Datatype upper_triangular_type;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Crear matriz de ejemplo (0 a n²-1)
        matrix = (int *)malloc(n * n * sizeof(int));
        for (int i = 0; i < n * n; i++) matrix[i] = i;

        printf("Matriz original:\n");
        print_matrix(matrix, n);

        // Calcular blocklengths y displacements para la triangular superior
        int *blocklengths = (int *)malloc(n * sizeof(int));
        int *displacements = (int *)malloc(n * sizeof(int));
        int k = 0;
        for (int i = 0; i < n; i++) {
            blocklengths[i] = n - i; // Elementos por fila: n, n-1, ..., 1
            displacements[i] = i * n + i; // Desplazamiento inicial por fila: 0, 5, 10, 15
        }

        // Crear tipo derivado
        MPI_Type_indexed(n, blocklengths, displacements, MPI_INT, &upper_triangular_type);
        MPI_Type_commit(&upper_triangular_type);

        // Enviar triangular superior al proceso 1
        MPI_Send(matrix, 1, upper_triangular_type, 1, 0, MPI_COMM_WORLD);

        free(blocklengths);
        free(displacements);
    } 
    else if (rank == 1) {
        // Calcular tamaño de la triangular superior (n*(n+1)/2)
        int triangular_size = n * (n + 1) / 2;
        triangular = (int *)malloc(triangular_size * sizeof(int));

        // Recibir datos
        MPI_Recv(triangular, triangular_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("\nTriangular superior recibida:\n");
        for (int i = 0; i < triangular_size; i++) {
            printf("%d ", triangular[i]);
        }
        printf("\n");
    }

    if (rank == 0) free(matrix);
    if (rank == 1) free(triangular);
    MPI_Type_free(&upper_triangular_type); // Liberar tipo derivado
    MPI_Finalize();
    return 0;
}