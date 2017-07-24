#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define n_filas         500
#define n_columnas      744
#define n_puntos_dn     1000
#define n_puntos_MC     100000
#define n_iteraciones   2000
#define INICIALIZAR     1
#define NO_INICIALIZAR  0
#define PI              3.14159265359
#define sigma           40
#define radio_max       10
#define R_tierra        6371

#define longitud(x) ((double)x/(double)n_columnas-(double)1/(double)2)*360;
#define latitud(y) -((double)y/(double)n_filas-(double)1/(double)2)*180;
#define rand_rango(vmin, rango) vmin+(double)rand()/((double)RAND_MAX + 1)*rango

double** crear_matriz(int inicializar);
void lectura_archivo(char *nombre_archivo, double** mapa);
double* crear_lista_normal();
double distancia_ortodromica(int x1, int y1, int x2, int y2);
void monte_carlo(double **mapa, double **distancia_maxima);
void promedio_mc(double **mapa, double **distancia_maxima);
int* encontrar_maximo(double **distancia_maxima);
void liberar_matriz(double** matriz);

int main() {
  char nombre_archivo[20] = "map_data.txt";
  double** mapa = crear_matriz(NO_INICIALIZAR);
  double** distancia_maxima = crear_matriz(INICIALIZAR);
  lectura_archivo(nombre_archivo, mapa);
  srand(time(NULL));
  promedio_mc(mapa, distancia_maxima);
  int* id_max = encontrar_maximo(distancia_maxima);
  double lon =  longitud(id_max[0]);
  double lat = latitud(id_max[1]);
  //printf("(%d,%d)\n",id_max[0],id_max[1]);
  printf("(%f,%f)\n",lat,lon);
  // for(int i = 0; i<n_filas; i++){
  //   for(int j = 0; j<n_columnas-1; j++)
  //     printf("%.0f, ", distancia_maxima[i][j]);
  //   printf("%.0f\n",  distancia_maxima[i][n_columnas-1]);
  // }
  free(id_max);
  liberar_matriz(mapa);
  liberar_matriz(distancia_maxima);

  return 0;
}
double** crear_matriz(int inicializar){
  double** matriz = malloc(n_filas*sizeof(double *));
  for(int i = 0 ; i < n_filas ; i++)
    matriz[i] = malloc(n_columnas*sizeof(double));

  if(inicializar)
    for(int i = 0 ; i < n_filas ; i++)
      for(int j = 0 ; j < n_columnas; j++)
        matriz[i][j] = 0;

  return matriz;
}
void lectura_archivo(char *nombre_archivo, double** mapa){
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
double* crear_lista_normal(){
  double r = rand_rango(0,n_filas), r_propuesto, f_r, f_r_propuesto, alpha, beta;
  double *lista_normal = malloc(n_puntos_dn*sizeof(double));
  lista_normal[0] = r;
  for(int i = 1; i<n_puntos_dn; i++){
    r_propuesto = lista_normal[i-1] + rand_rango(-radio_max,2*radio_max);
    f_r = exp(-lista_normal[i-1]*lista_normal[i-1]/(2*sigma*sigma));
    f_r_propuesto = exp(-r_propuesto*r_propuesto/(2*sigma*sigma));
    alpha = f_r_propuesto/f_r;
    //printf("%f\t", alpha);
    if(alpha>1)
      lista_normal[i] = r_propuesto;
    else{
      beta = rand_rango(0,1);
      if(alpha > beta)
        lista_normal[i] = r_propuesto;
      else
        lista_normal[i] = lista_normal[i-1];
    }
    //printf("%d, %f\t", i, lista_normal[i]);
  }
  return lista_normal;
}
double distancia_ortodromica(int x1, int y1, int x2, int y2){
  double lon1 = longitud(x1), lat1 = latitud(y1);
  double lon2 = longitud(x2), lat2 = latitud(y2);
  double raiz_smvseno_lat = sin((double)abs(lat1-lat2)/(double)2);
  double raiz_smvseno_lon = sin((double)abs(lon1-lon2)/(double)2);
  double semivseno_lat = raiz_smvseno_lat*raiz_smvseno_lat;
  double semivseno_lon = raiz_smvseno_lon*raiz_smvseno_lon;
  double ang_central = 2*asin(semivseno_lat+cos(lat1)*cos(lat2)*semivseno_lon);
  double distancia = R_tierra * ang_central;
  return distancia;
}
void monte_carlo(double** mapa, double** distancia_maxima){
  int x, y, rand_id_x, rand_id_y, x_nuevo, y_nuevo;
  double dx, dy, distancia, alpha, beta;
  do {
    x = (int)rand_rango(0,n_columnas);
    y = (int)rand_rango(0,n_filas);
  } while(mapa[y][x] == 1);
  double* lista_normal = crear_lista_normal();
  for(int i = 1; i<n_puntos_MC; i++){
    do {
      rand_id_x = (int)rand_rango(0,n_puntos_dn);
      rand_id_y = (int)rand_rango(0,n_puntos_dn);
      dx = lista_normal[rand_id_x];
      dy = lista_normal[rand_id_y];
    } while((int)dx!=0 && (int)dy!=0);
    //printf("%d,%d\t", x,y);
    //printf("%d\t", ((int)(x+dx))%n_columnas);
    if((int)(x+dx) >= n_columnas){
      x_nuevo = ((int)(x+dx))%n_columnas;
    }
    else if ((int)(x+dx) < 0){
      x_nuevo = n_columnas + ((int)(x+dx))%n_columnas;
    }
    else {
      x_nuevo =(int)(x+dx);
    }
    if((int)(y+dy) >= n_filas){
      y_nuevo = ((int)(y+dy))%n_filas;
    }
    else if ((int)(y+dy) < 0){
      y_nuevo = n_filas + ((int)(y+dy))%n_filas;
    }
    else {
      y_nuevo = (int)(y+dy);
    }
    distancia = distancia_ortodromica(x, y, x_nuevo, y_nuevo);
    if(mapa[y_nuevo][x_nuevo] == 0){
      
      alpha = distancia_maxima[y_nuevo][x_nuevo]/distancia_maxima[y][x];
      if(alpha>1){
        x = x_nuevo;
        y = y_nuevo;
      }
      else{
        beta = rand_rango(0,1);
        if(alpha > beta){
          x = x_nuevo;
          y = y_nuevo;
        }
      }
    }
    else{
      if(distancia_maxima[y][x] == 0 || distancia_maxima[y][x] > distancia)
        distancia_maxima[y][x] == distancia;
      do {
        x = (int)rand_rango(0,n_columnas);
        y = (int)rand_rango(0,n_filas);
      } while(mapa[y][x] == 1);
    }
  }
  //printf("%f\t", lista_normal[400]);
  //printf("\n");
  free(lista_normal);
}
void promedio_mc(double **mapa, double **distancia_maxima){
  for(int i=0;i<n_iteraciones;i++){
    double** distancia_maxima_iteracion = crear_matriz(INICIALIZAR);
    monte_carlo(mapa, distancia_maxima_iteracion);
    for(int i = 0 ; i < n_filas ; i++)
      for(int j = 0 ; j < n_columnas; j++)
        distancia_maxima[i][j] += distancia_maxima_iteracion[i][j];
    liberar_matriz(distancia_maxima_iteracion);
  }
  for(int i = 0 ; i < n_filas ; i++)
    for(int j = 0 ; j < n_columnas; j++)
      distancia_maxima[i][j] = (double)distancia_maxima[i][j]/(double)n_iteraciones;
}
int* encontrar_maximo(double** distancia_maxima){
  int* indices = malloc(2*sizeof(int));
  double maximo;
  indices[0] = 0;
  indices[1] = 0;
  maximo = distancia_maxima[0][0];
  for(int i = 0 ; i < n_filas ; i++)
    for(int j = 0 ; j < n_columnas; j++)
      if(distancia_maxima[i][j] > maximo){
        indices[0] = j;
        indices[1] = i;
        maximo = distancia_maxima[i][j];
      }
  return indices;
}
void liberar_matriz(double** matriz){
    for(int i = 0 ; i < n_filas ; i++)
        free(matriz[i]);
    free(matriz);
}
