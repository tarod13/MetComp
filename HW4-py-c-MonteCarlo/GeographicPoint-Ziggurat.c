#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define n_filas 500
#define n_columnas 744
#define n_puntos 100000
#define INICIALIZAR 1
#define PI 3.14159265359
#define rand_rango(rango) (double)rand() / ((double)RAND_MAX + 1) * rango

int** crear_matriz(int inicializar);
void lectura_archivo(char *nombre_archivo, int** mapa);
void rand_normal(float **num_aleatorios, int n_fil, int n_col);
void monte_carlo(int **mapa, int **funcion_valor);
void liberar_matriz(int** matriz);

int main() {
  char nombre_archivo[20] = "map_data.txt";
  float **num_aleatorios;
  int** mapa = crear_matriz(!INICIALIZAR);
  int** funcion_valor = crear_matriz(INICIALIZAR);
  for(int i = 20; i<40; i++){
    for(int j = 20; j<39; j++)
      printf("%d, ", funcion_valor[i][j]);
    printf("%d\n",  funcion_valor[i][40]);
  }
  lectura_archivo(nombre_archivo, mapa);
  srand(time(NULL));

  liberar_matriz(mapa);
  liberar_matriz(funcion_valor);

  return 0;
}
int** crear_matriz(int inicializar){
  int** matriz = malloc(n_filas*sizeof(int *));
  for(int i = 0 ; i < n_filas ; i++)
    matriz[i] = malloc(n_columnas*sizeof(int));

  if(inicializar)
    for(int i = 0 ; i < n_filas ; i++)
      for(int j = 0 ; j < n_columnas; j++)
        matriz[i][j] = 0;

  return matriz;
}
void lectura_archivo(char *nombre_archivo, int** mapa){
  FILE *archivo = fopen(nombre_archivo, "r");
  int max_long = 2048, n_fila = 0, n_columna = 0;
  char linea[max_long];
  while (fgets(linea, max_long, archivo)){
      char* copia_linea = strdup(linea);
      mapa[n_fila][n_columna] = strtod(strtok(copia_linea, " "),NULL);
      while(n_columna<n_columnas-1){
        n_columna++;
      	mapa[n_fila][n_columna] = strtod(strtok(NULL, " "),NULL);
      }
      free(copia_linea);
      n_fila ++;
      n_columna = 0;
  }
  fclose(archivo);
}
void rand_normal(float **num_aleatorios, int n_fil, int n_col, double sigma){
  int n_rectangulos = 2048;
  double *x, *y;
  double divisor_normal = sqrt(2*PI)*sigma;
  double area = 0.5/n_rectangulos;

  x = malloc(n_rectangulos*sizeof(int));
  y = malloc(n_rectangulos*sizeof(int));

  x[0] = 10000*sigma;
  x[1] = x[0];
  y[0] = exp(-x[0]*x[0]/(sigma*sigma)/(divisor_normal);
  for(int i = 1; i<n_rectangulos-1; i++){
    y[i] = y[i-1] + area/x[i]
    x[i+1] = sigma*sqrt(-2*log(divisor_normal*y[i]));
  }
  y[n_rectangulos-1] = 1/divisor_normal;

  for(int i = 0; i<n_fil; i++)
    for(int j = 0; j<n_col; j++){
      int no_aceptado = 1;
      do {
        int id = (int)rand_rango(n_rectangulos);
        if(id==0){
          double w = rand_rango(area);
          if(w<x[0]*y[0])
            num_aleatorios[i][j] = (int)(w/y[0]);
          else{
            double u1, u2;
            do {
              u1 = rand_rango(1);
            } while(u1==0);
            do {
              u2 = rand_rango(1);
            } while(u2==0);
            double a = -log(u1)/x[0];
            double b = -log(u2);
            if(2*b > a*a)
              num_aleatorios[i][j] = (int)(a+x[0]);
          }
        }
        else{
          double rand_x = (int)rand_rango(x[id]);
          if(id < n_rectangulos-1)
            if(rand_x < x[id+1])
              num_aleatorios[i][j] = (int)rand_x;
          else{
            double dy = y[id] - y[id-1];
            double rand_y = y[id-1] + (int)rand_rango(dy);
            double normal_y = exp(-rand_x*rand_x/(sigma*sigma)/(divisor_normal);
            if(normal_y>rand_y)
              num_aleatorios[i][j] = (int)rand_x;
          }
        }
      } while(no_aceptado);
    }
}
void monte_carlo(int** mapa, int** funcion_valor){
  int continente = 1, x, y;
  while(continente == 1){
    x = (int)rand_rango(n_columnas);
    y = (int)rand_rango(n_filas);
    continente = mapa[y][x];
  }


}
void liberar_matriz(int** matriz){
    for(int i = 0 ; i < n_filas ; i++)
        free(matriz[i]);
    free(matriz);
}
