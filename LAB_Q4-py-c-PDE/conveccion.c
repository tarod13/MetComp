#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define n_puntos_x  100
#define n_puntos_t  300
#define c           1
#define xfin        2
#define tfin        0.3
#define dt          (double)tfin/(double)(n_puntos_t-1)
#define dx          (double)xfin/(double)(n_puntos_x-1)
#define gamma       ((double)(c*dt))/((double)dx)

double** crear_matriz();
void condicion_inicial(double** u);
void evolucion(double** u);
void guardar_matriz(double** u, char* nombre_archivo);
void liberar_matriz(double** u);

int main() {
  char nombre_archivo[20] = "conveccion.csv";
  double** u = crear_matriz();
  printf("%f,%f,%f\n",dt,dx,gamma);
  condicion_inicial(u);
  evolucion(u);
  guardar_matriz(u, nombre_archivo);
  liberar_matriz(u);
  return 0;
}
double** crear_matriz(){
  double** matriz = malloc(n_puntos_t*sizeof(double *));
  for(int i = 0 ; i < n_puntos_t ; i++)
    matriz[i] = malloc(n_puntos_x*sizeof(double));

  for(int i = 0 ; i < n_puntos_t ; i++)
    for(int j = 0 ; j < n_puntos_x; j++)
      matriz[i][j] = 0.0;

  return matriz;
}
void condicion_inicial(double** u){
  double x1 = 0.7, x2 = 1.2, u0 = 2.0;
  for(int i = 0 ; i < n_puntos_x; i++){
    double x = (double)i/(double)(n_puntos_x-1)*2.0;
    if(x > x1 && x < x2)
      u[0][i] = u0;
  }

  for(int i = 1 ; i < n_puntos_x-1; i++)
    u[1][i] = u[0][i] - (u[0][i+1]-u[0][i-1])*gamma/2.0;
}
void evolucion(double** u){
  for(int i = 2 ; i < n_puntos_t; i++)
    for(int j = 1 ; j < n_puntos_x-1; j++)
      u[i][j] = u[i-2][j] - (u[i-1][j+1]-u[i-1][j-1])*gamma;
}
void guardar_matriz(double** u, char* nombre_archivo){
	FILE *archivo = fopen(nombre_archivo, "w");
	for(int t=0;t<n_puntos_t;t++){
		for(int x=0;x<n_puntos_x-1;x++)
		  fprintf(archivo, "%f,", u[t][x]);
		fprintf(archivo, "%f\n", u[t][n_puntos_x-1]);
  }
	fclose(archivo);
}
void liberar_matriz(double** matriz){
    for(int i = 0 ; i < n_puntos_t ; i++)
        free(matriz[i]);
    free(matriz);
}
