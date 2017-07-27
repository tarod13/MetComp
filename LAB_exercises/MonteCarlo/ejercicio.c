#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define n_puntos_t  1000
#define gamma       0.5
#define N0          10.0
#define iteraciones 1000
#define tfin        10
#define dt          (double)tfin/(double)(n_puntos_t-1)

void condicion_inicial(double* N);
void evolucion(double* N);
void evolucion_MC(double** Nrand);
void guardar_vector(double* N, char* nombre_archivo);
void guardar_matriz(double** N, char* nombre_archivo);
void liberar_matriz(double** matriz);

int main() {
  srand(time(NULL));
  char nombre_archivo_1[20] = "MC.csv";
  char nombre_archivo_2[20] = "MC_rand.csv";
  double* N = malloc(n_puntos_t*sizeof(double));
  double** Nrand = malloc(n_puntos_t*sizeof(double*));
  for(int it = 0; it<iteraciones; it++)
    Nrand[it] = malloc(n_puntos_t*sizeof(double));
  condicion_inicial(N);
  evolucion(N);
  evolucion_MC(Nrand);
  guardar_vector(N, nombre_archivo_1);
  guardar_matriz(Nrand, nombre_archivo_2);
  free(N);
  liberar_matriz(Nrand);
  return 0;
}
void condicion_inicial(double* N){
  N[0] = N0;
  N[1] = N[0]*(1 - dt*gamma);
}
void evolucion(double* N){
  int i;
  for(i = 2 ; i < n_puntos_t; i++)
    N[i] = N[i-2] - 2.0*dt*gamma*N[i-1];
}
void evolucion_MC(double** Nrand){
  for(int k = 0; k<iteraciones; k++){
    Nrand[k][0] = N0;
    for(int j = 1 ; j < n_puntos_t; j++){
      Nrand[k][j] = Nrand[k][j-1];
      double random = ((double)rand()/((double)RAND_MAX + 1));
      if(random < (double)gamma*dt*Nrand[k][j-1])
        Nrand[k][j]--;
      }
  }
}
void guardar_vector(double* N, char* nombre_archivo){
	FILE *archivo = fopen(nombre_archivo, "w");
	for(int t=0;t<n_puntos_t-1;t++){
		fprintf(archivo, "%f,", N[t]);
  }
	fclose(archivo);
}
void guardar_matriz(double** u, char* nombre_archivo){
	FILE *archivo = fopen(nombre_archivo, "w");
	for(int h=0;h<iteraciones;h++){
		for(int t=0;t<n_puntos_t-1;t++)
		  fprintf(archivo, "%f,", u[h][t]);
		fprintf(archivo, "%f\n", u[h][n_puntos_t-1]);
  }
	fclose(archivo);
}
void liberar_matriz(double** matriz){
    for(int i = 0 ; i < n_puntos_t ; i++)
        free(matriz[i]);
    free(matriz);
}
