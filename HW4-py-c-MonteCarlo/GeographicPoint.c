#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define n_filas         500
#define n_columnas      744
#define n_puntos_dn     1000
#define n_puntos_MC     10000
#define n_iteraciones   1
#define INICIALIZAR     1
#define NO_INICIALIZAR  0
#define PI              3.14159265359
#define sigma           2
#define radio_max       20
#define R_tierra        6371

#define longitud(x) ((double)x/(double)n_columnas-(double)1/(double)2)*360.0;
#define latitud(y) -((double)y/(double)n_filas-(double)1/(double)2)*180.0;
#define rand_rango(vmin, rango) vmin+(double)rand()/((double)RAND_MAX + 1)*rango

double** crear_matriz(int inicializar, double valor);
void lectura_archivo(char *nombre_archivo, double** mapa);
double* crear_lista_normal();
double distancia_ortodromica(int x1, int y1, int x2, int y2);
void monte_carlo(double **mapa, double **dist_min, double **dist_max);
void promedio_mc(double **mapa, double **dist_min);
int* encontrar_maximo(double **dist_min);
void liberar_matriz(double** matriz);

int main() {
  char nombre_archivo[20] = "map_data.txt";
  double** mapa = crear_matriz(NO_INICIALIZAR,0);
  double** distancia_minima = crear_matriz(INICIALIZAR,0.0);
  lectura_archivo(nombre_archivo, mapa);
  srand(time(NULL));
  promedio_mc(mapa, distancia_minima);
  int* id_max = encontrar_maximo(distancia_minima);
  double dist = distancia_minima[id_max[1]][id_max[0]];
  double lon =  longitud(id_max[0]);
  double lat = latitud(id_max[1]);
  //printf("(%d,%d)\n",id_max[0],id_max[1]);
  // for(int i = 0; i<n_filas; i++){
  //   for(int j = 0; j<n_columnas-1; j++)
  //     printf("%.0f, ", distancia_minima[i][j]);
  //   printf("%.0f\n",  distancia_minima[i][n_columnas-1]);
  // }
  printf("(%f,%f;%f)\n",lat,lon,dist);
  free(id_max);
  liberar_matriz(mapa);
  liberar_matriz(distancia_minima);

  return 0;
}
double** crear_matriz(int inicializar, double valor){
  double** matriz = malloc(n_filas*sizeof(double *));
  for(int i = 0 ; i < n_filas ; i++)
    matriz[i] = malloc(n_columnas*sizeof(double));

  if(inicializar)
    for(int i = 0 ; i < n_filas ; i++)
      for(int j = 0 ; j < n_columnas; j++)
        matriz[i][j] = valor;

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
  double lon1 = (double)PI/(double)180*(double)longitud(x1);
  double lat1 = (double)PI/(double)180*(double)latitud(y1);
  double lon2 = (double)PI/(double)180*(double)longitud(x2);
  double lat2 = (double)PI/(double)180*(double)latitud(y2);
  double raiz_smvseno_lat = (double)sin((double)(lat1-lat2)/(double)2);
  double raiz_smvseno_lon = (double)sin((double)(lon1-lon2)/(double)2);
  double semivseno_lat = (double)(raiz_smvseno_lat*raiz_smvseno_lat);
  double semivseno_lon = (double)(raiz_smvseno_lon*raiz_smvseno_lon);
  double s2 = (double)cos(lat1)*(double)cos(lat2)*(double)semivseno_lon;
  double ang_central = (double)2*(double)(asin(semivseno_lat+s2));
  double distancia = (double)R_tierra * (double)ang_central;
  //printf("(%.3f,%.3f;%.3f)\t", lat1,lat2,semivseno_lat);
  //printf("(%.4f,%.4f,%.4f,%.4f;%.4f)\t", lat1,lon1,lat2,lon2,distancia);
  return distancia;
}
void monte_carlo(double** mapa, double** dist_min, double** dist_max){
  int x, y, rand_id_x, rand_id_y, x_nuevo, y_nuevo;
  double dx, dy, dist, alpha, beta;
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
    dist = distancia_ortodromica(x, y, x_nuevo, y_nuevo);
    if(mapa[y_nuevo][x_nuevo] == 0){
      if(dist_min[y][x] < dist && dist < dist_max[y][x])
        dist_min[y][x] = dist;
      if(dist_min[y_nuevo][x_nuevo] < dist && dist < dist_max[y_nuevo][x_nuevo])
        dist_min[y_nuevo][x_nuevo] = dist;
      alpha = dist_min[y_nuevo][x_nuevo]/dist_min[y][x];
      printf("%.2f\t",alpha);
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
      if(dist_max[y][x] > dist)
        dist_max[y][x] = dist;
      if(dist_min[y][x] > dist_max[y][x])
        dist_min[y][x] = dist_max[y][x];
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
void promedio_mc(double **mapa, double **dist_min){
  double** dist_max = crear_matriz(INICIALIZAR,PI*R_tierra);
  for(int i=0;i<n_iteraciones;i++){
    double** dist_min_iteracion = crear_matriz(INICIALIZAR,0.0);
    double** dist_max_iteracion = crear_matriz(INICIALIZAR,PI*R_tierra);
    monte_carlo(mapa, dist_min_iteracion, dist_max_iteracion);
    for(int i = 0 ; i < n_filas ; i++)
      for(int j = 0 ; j < n_columnas; j++){
        if(dist_max_iteracion[i][j] < dist_max[i][j]){
          dist_max[i][j] = dist_max_iteracion[i][j];
          if(dist_max[i][j] < dist_min[i][j])
            dist_min[i][j] = dist_max[i][j];
        }
        if(dist_min_iteracion[i][j] > dist_min[i][j] &&
                                    dist_min_iteracion[i][j] < dist_max[i][j])
          dist_min[i][j] = dist_min_iteracion[i][j];
      }
    liberar_matriz(dist_min_iteracion);
    liberar_matriz(dist_max_iteracion);
  }
  liberar_matriz(dist_max);
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
