#include <stdio.h>
#include "mpi.h"

#define MAX 10
#define MAESTRO 0
#define RECIBE 1
#define OPERA 2

void imprime_vector(int vector[], int maximo){
  int i;
  for(i = 0; i<maximo; i++){
     printf("%d\n",vector[i]);
  }
}
void main(){
  int a = 0;
int b = 1;
int cont = 1;
     int f= 0;
  MPI_Init(NULL, NULL);
  MPI_Status status;
  int nro_procesadores;
  MPI_Comm_size(MPI_COMM_WORLD, &nro_procesadores);
  int nro_ejecuta;
  MPI_Comm_rank(MPI_COMM_WORLD, &nro_ejecuta);

  int maes= 0;
  char cad[] = "cadena";
  printf("%d %d\n",nro_procesadores, nro_ejecuta);
  if(nro_ejecuta == maes){
     int i;
     for (i = 0; i < 5; i++){ 
        MPI_Send(&a, 1, MPI_INT, RECIBE, 20, MPI_COMM_WORLD);
        MPI_Send(&b, 1, MPI_INT, RECIBE, 30, MPI_COMM_WORLD); 
     }
  }
  int i;
  for (i = 0; i < 5; i++){
    if(nro_ejecuta == RECIBE){
      MPI_Recv(&a, 1, MPI_INT, maes, 20, MPI_COMM_WORLD, &status);
      MPI_Recv(&b, 1, MPI_INT, maes, 30, MPI_COMM_WORLD, &status);
      if(maes == 0){
         maes = 2;
      }
      
      //int c = &a + &b;
      f = a + b;
      printf("%d\n", f);
      MPI_Send(&b, 1, MPI_INT, OPERA, 20, MPI_COMM_WORLD);
      MPI_Send(&f, 1, MPI_INT, OPERA, 30, MPI_COMM_WORLD);
    }

    if(nro_ejecuta == OPERA){
      MPI_Recv(&a, 1, MPI_INT, RECIBE, 20, MPI_COMM_WORLD, &status);
      MPI_Recv(&b, 1, MPI_INT, RECIBE, 30, MPI_COMM_WORLD, &status);
      f = a + b;
      printf("%d\n", f);
      MPI_Send(&b, 1, MPI_INT, RECIBE, 20, MPI_COMM_WORLD);
      MPI_Send(&f, 1, MPI_INT, RECIBE, 30, MPI_COMM_WORLD);
    }
  }


  MPI_Finalize();
  
}
