#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define n_filas         500
#define n_columnas      744
#define n_columnas_o    744
#define n_puntos_dn     100000
#define n_puntos_MC     15000
#define n_rep_rad       900
#define n_iteraciones   60
#define ORIGINAL        1
#define MODIFICADA      0
#define INICIALIZAR     1
#define NO_INICIALIZAR  0
#define PI              3.14159265359
#define sigma           160
#define radio_max       20
#define R_tierra        6371

#define longitud(x) (((double)x/(double)n_columnas)-((double)1/(double)2))*360.0
#define latitud(y) -(((double)y/(double)n_filas)-((double)1/(double)2))*180.0
#define rand_rango(vmin, rango) vmin+((double)rand()/((double)RAND_MAX + 1))*rango

double** crear_matriz(int inicializar, double valor, int original);
void lectura_archivo(char *nombre_archivo, double** mapa);
double* crear_lista_normal();
double distancia_ortodromica(int x1, int y1, int x2, int y2);
int* buscar_nueva_posicion(int x,int y,double* lista_normal);
void estimar_radio(int x, int y, double* lista_normal, double** mapa,
                                        double** dist_min, double** dist_max);
void monte_carlo(double** mapa, double** d_min, double** d_max, double* lista_normal);
void promedio_mc(double **mapa, double **dist_min, double* lista_normal);
int* encontrar_maximo(double **dist_min);
void guardar_matriz(double** matriz, char* nombre_archivo);
void liberar_matriz(double** matriz);

int main() {
  srand(time(NULL));
  char nombre_archivo_lectura[20] = "map_data.txt";
  char nombre_archivo_escritura_min[20] = "map_dist_min.txt";
  printf("Creando matrices...\n");
  double** mapa = crear_matriz(NO_INICIALIZAR,0,MODIFICADA);
  double** distancia_minima = crear_matriz(INICIALIZAR,0.0,MODIFICADA);
  double* lista_normal = crear_lista_normal();
  //double** dist_max = crear_matriz(INICIALIZAR,PI*R_tierra);
  printf("Leyendo archivo...\n");
  lectura_archivo(nombre_archivo_lectura, mapa);
  printf("Calculando punto mas lejano\n");
  promedio_mc(mapa, distancia_minima, lista_normal);
  int* id_max = encontrar_maximo(distancia_minima);
  double dist = distancia_minima[id_max[1]][id_max[0]];
  double dist2 = distancia_minima[383][117];
  double lon =  longitud(id_max[0]);
  double lat = latitud(id_max[1]);
  //printf("(%d,%d)\n",id_max[0],id_max[1]);
  // for(int i = 400; i<n_filas; i++){
  //   for(int j = 644; j<n_columnas-1; j++)
  //     printf("%.0f, ", distancia_minima[i][j]);
  //   printf("%.0f\n",  distancia_minima[i][n_columnas-1]);
  // }
  printf("Latitud = %f, Longitud = %f; Radios estimados = %f, %f\n",lat,lon,dist,dist2);
  guardar_matriz(distancia_minima,nombre_archivo_escritura_min);
  liberar_matriz(mapa);
  liberar_matriz(distancia_minima);
  free(id_max);
  free(lista_normal);
  //liberar_matriz(dist_max);
  return 0;
}
double** crear_matriz(int inicializar, double valor, int original){
  int n_col;
  if(original)
    n_col = n_columnas_o;
  else
    n_col = n_columnas;
  double** matriz = malloc(n_filas*sizeof(double *));
  for(int i = 0 ; i < n_filas ; i++)
    matriz[i] = malloc(n_col*sizeof(double));
  if(inicializar)
    for(int i = 0 ; i < n_filas ; i++)
      for(int j = 0 ; j < n_col; j++)
        matriz[i][j] = valor;
  return matriz;
}
void lectura_archivo(char *nombre_archivo, double** mapa){
  FILE *archivo = fopen(nombre_archivo, "r");
  double** mapa_original = crear_matriz(NO_INICIALIZAR,0,ORIGINAL);
  int max_long = 2048, n_fila = 0, n_columna = 0;
  char linea[max_long];
  while (fgets(linea, max_long, archivo)){
      char* copia_linea = strdup(linea);
      mapa_original[n_fila][n_columna] = strtod(strtok(copia_linea, " "),NULL);
      while(n_columna<n_columnas-1){
        n_columna++;
      	mapa_original[n_fila][n_columna] = strtod(strtok(NULL, " "),NULL);
      }
      free(copia_linea);
      n_fila ++;
      n_columna = 0;
  }
  fclose(archivo);

  for(int i = 0 ; i < n_filas ; i++)
    for(int j = 0 ; j < n_columnas; j++)
      mapa[i][j] = mapa_original[i][j+(n_columnas_o-n_columnas)];
  liberar_matriz(mapa_original);
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
int* buscar_nueva_posicion(int x, int y, double* lista_normal){
  int rand_id_x, rand_id_y;
  double dx, dy;
  int* nueva_pos = malloc(2*sizeof(int));

  do {
    rand_id_x = (int)rand_rango(0,n_puntos_dn);
    rand_id_y = (int)rand_rango(0,n_puntos_dn);
    dx = lista_normal[rand_id_x];
    dy = lista_normal[rand_id_y]/2.0;

    if((int)(x+dx) >= n_columnas){
      nueva_pos[0] = ((int)(x+dx))%n_columnas;
    }
    else if ((int)(x+dx) < 0){
      nueva_pos[0] = n_columnas + ((int)(x+dx))%n_columnas;
    }
    else {
      nueva_pos[0] =(int)(x+dx);
    }
    if((int)(y+dy) >= n_filas){
      nueva_pos[1] = ((int)(y+dy))%n_filas;
    }
    else if ((int)(y+dy) < 0){
      nueva_pos[1] = n_filas + ((int)(y+dy))%n_filas;
    }
    else {
      nueva_pos[1] = (int)(y+dy);
    }
  } while(x==nueva_pos[0] && y==nueva_pos[1]);

  return nueva_pos;
}
void estimar_radio(int x, int y, double* lista_normal, double** mapa,
                                        double** dist_min, double** dist_max){
  for(int rep = 0; rep < n_rep_rad; rep++){
    int *nueva_pos = buscar_nueva_posicion(x,y,lista_normal);
    double dist = distancia_ortodromica(x, y, nueva_pos[0], nueva_pos[1]);

    if(mapa[nueva_pos[1]][nueva_pos[0]] == 0){
      //printf("a\t");
      if(dist < dist_max[y][x]){
        dist_min[y][x] += dist;
        //printf("%.3f\t", dist);
      }
    }
    else{
      //printf("b\t");
      if(dist_max[y][x] > dist)
        dist_max[y][x] = dist;
    }
    free(nueva_pos);
  }
  dist_min[y][x] = ((double)dist_min[y][x])/((double)n_rep_rad);
  if(dist_min[y][x] > dist_max[y][x])
      dist_min[y][x] = dist_max[y][x];
  //printf("\n");
}
void monte_carlo(double** mapa, double** d_min, double** d_max, double* lista_normal){
  int x, y, rand_id_x, rand_id_y, *nueva_pos;
  double dx, dy, dist, alpha, beta;
  do {
    x = (int)rand_rango(0,n_columnas);
    y = (int)rand_rango(0,n_filas);
  } while(mapa[y][x] == 1);
  estimar_radio(x,y,lista_normal,mapa,d_min,d_max);
  for(int i = 1; i<n_puntos_MC; i++){
    do {
    nueva_pos = buscar_nueva_posicion(x,y,lista_normal);
  } while(mapa[nueva_pos[1]][nueva_pos[0]] == 1);
    estimar_radio(nueva_pos[0],nueva_pos[1],lista_normal,mapa,d_min,d_max);
    alpha = d_min[nueva_pos[1]][nueva_pos[0]]/d_min[y][x];
    //printf("%.2f\t",alpha);
    if(alpha>1){
      x = nueva_pos[0];
      y = nueva_pos[1];
    }
    else{
      beta = rand_rango(0,1);
      if(alpha > beta){
        x = nueva_pos[0];
        y = nueva_pos[1];
      }
    }
    free(nueva_pos);
  }
  // //printf("%f\t", lista_normal[400]);
  // printf("\n");
}
void promedio_mc(double **mapa, double **dist_min, double *lista_normal){
  char nombre_archivo_escritura_max[20] = "map_dist_max.txt";
  double** dist_max = crear_matriz(INICIALIZAR,PI*R_tierra,MODIFICADA);
  for(int i=0;i<n_iteraciones;i++){
    printf("Iteracion %d de %d...\n",i+1,n_iteraciones);
    double** dist_min_iteracion = crear_matriz(INICIALIZAR,0.0,MODIFICADA);
    monte_carlo(mapa, dist_min_iteracion, dist_max, lista_normal);
    for(int i = 0 ; i < n_filas ; i++)
      for(int j = 0 ; j < n_columnas; j++)
        dist_min[i][j] += ((double)dist_min_iteracion[i][j])\
                                                  /((double)n_iteraciones);
    liberar_matriz(dist_min_iteracion);
  }
  guardar_matriz(dist_max,nombre_archivo_escritura_max);
  liberar_matriz(dist_max);
}
int* encontrar_maximo(double** distancia_maxima){
  int *indices = malloc(2*sizeof(int));
  double maximo;
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
void guardar_matriz(double** matriz, char* nombre_archivo){
	FILE *archivo = fopen(nombre_archivo, "w");
	for(int i=0;i<n_filas;i++){
		for(int j=0;j<n_columnas-1;j++)
		  fprintf(archivo, "%f,", matriz[i][j]);
		fprintf(archivo, "%f\n", matriz[i][n_columnas-1]);
  }
	fclose(archivo);
}

void liberar_matriz(double** matriz){
    for(int i = 0 ; i < n_filas ; i++)
        free(matriz[i]);
    free(matriz);
}
