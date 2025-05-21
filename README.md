Este proyecto implementa en C/MPI un programa que extrae y comunica la parte triangular superior de una matriz cuadrada utilizando tipos de datos derivados (MPI_Type_indexed). El proceso 0 envía la triangular superior al proceso 1 mediante una sola operación de comunicación colectiva, demostrando el manejo eficiente de patrones de datos no contiguos en MPI.

🔧 Requisitos
Compilador de C (gcc, clang).

Biblioteca MPI (OpenMPI o MPICH).

Sistema Linux (recomendado).

Compilar :

mpicc triangular_mpi.c -o triangular_mpi  


Ejemplo que se espera:

Matriz original:  
 0  1  2  3  
 4  5  6  7  
 8  9 10 11  
12 13 14 15  

Triangular superior recibida:  
0 1 2 3 5 6 7 10 11 15  
