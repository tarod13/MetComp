#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define n_filas         500
#define n_columnas      709
#define n_columnas_f    28
#define n_columnas_o    744
#define n_puntos_dn     100000
#define n_puntos_MC     100001
#define exp_est_rad     1
#define exp_est_rad_max 1
#define n_iteraciones   1
#define ORIGINAL        1
#define MODIFICADA      0
#define PI              3.14159265359
#define sigma           160
#define radio_max_n     20
#define radio_max_u     50
#define R_tierra        6371

#define longitud(x) (((double)x/(double)n_columnas)-((double)1/(double)2))*360.0
#define latitud(y) -(((double)y/(double)n_filas)-((double)1/(double)2))*180.0
#define rand_rango(vmin, rango) vmin+((double)rand()/((double)RAND_MAX + 1))*rango

double** crear_matriz(double valor, int original);
void lectura_archivo(char *nombre_archivo, double** mapa);
double* crear_lista_normal();
double distancia_ortodromica(int x1, int y1, int x2, int y2);
int* buscar_nueva_posicion_n(int x,int y,double* lista_normal);
int* buscar_nueva_posicion_u(int x,int y,double radio);
void estimar_radio(int x, int y, double** mapa, int n, double** dist_max);
void monte_carlo(double** mapa, double** d_max, double* lista_normal);
void promedio_mc(double **mapa, double **d_max, double* lista_normal);
int* encontrar_maximo(double **dist_max);
void guardar_maximo(int* max_id, double dist, char* nombre_archivo);
void liberar_matriz(double** matriz);

int main() {
  srand(time(NULL));

  char nombre_archivo_lectura[20] = "map_data.txt";
  char nombre_archivo_escritura[20] = "pmax.txt";

  double** mapa = crear_matriz(0.0,MODIFICADA);
  double** distancia_maxima = crear_matriz(0.0,MODIFICADA);
  double* lista_normal = crear_lista_normal();

  lectura_archivo(nombre_archivo_lectura, mapa);

  printf("Calculando punto mas lejano\n");
  promedio_mc(mapa, distancia_maxima, lista_normal);

  int* id_max = encontrar_maximo(distancia_maxima);
  //estimar_radio(id_max[0],id_max[1],mapa,exp_est_rad_max,distancia_maxima);
  double dist = distancia_maxima[id_max[1]][id_max[0]];
  double lon =  longitud(id_max[0]);
  double lat = latitud(id_max[1]);
  printf("las coordenadas del punto mas alejado son: %f, %f\n",lon,lat);
  guardar_maximo(id_max, dist, nombre_archivo_escritura);

  liberar_matriz(mapa);
  liberar_matriz(distancia_maxima);
  free(id_max);
  free(lista_normal);

  return 0;
}
double** crear_matriz(double valor, int original){
  int i, j, n_col;
  if(original)
    n_col = n_columnas_o;
  else
    n_col = n_columnas;
  double** matriz = malloc(n_filas*sizeof(double *));
  for(i = 0 ; i < n_filas ; i++)
    matriz[i] = malloc(n_col*sizeof(double));

  for(i = 0 ; i < n_filas ; i++)
    for(j = 0 ; j < n_col; j++)
      matriz[i][j] = valor;

  return matriz;
}
void lectura_archivo(char *nombre_archivo, double** mapa){
  FILE *archivo = fopen(nombre_archivo, "r");
  double** mapa_original = crear_matriz(0.0,ORIGINAL);
  int max_long = 2048, n_fila = 0, n_columna = 0, i, j;
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

  for(i = 0 ; i < n_filas ; i++)
    for(j = 0 ; j < n_columnas; j++)
      mapa[i][j] = mapa_original[i][j+n_columnas_f];
  liberar_matriz(mapa_original);
}
double* crear_lista_normal(){
  int i;
  double r = rand_rango(0,n_filas), r_propuesto, f_r, f_r_propuesto, alpha, beta;
  double *lista_normal = malloc(n_puntos_dn*sizeof(double));
  lista_normal[0] = r;
  for(i = 1; i<n_puntos_dn; i++){
    r_propuesto = lista_normal[i-1] + rand_rango(-radio_max_n,2.0*radio_max_n);
    f_r = exp(-(double)(lista_normal[i-1]*lista_normal[i-1])/(double)(2.0*sigma*sigma));
    f_r_propuesto = exp(-(double)(r_propuesto*r_propuesto)/(double)(2.0*sigma*sigma));

    alpha = f_r_propuesto/f_r;
    if(alpha>1)
      lista_normal[i] = r_propuesto;
    else{
      beta = rand_rango(0,1);
      if(alpha > beta)
        lista_normal[i] = r_propuesto;
      else
        lista_normal[i] = lista_normal[i-1];
    }
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
  double raiz = (double)sqrt(semivseno_lat+s2);
  double ang_central = (double)2*(double)(asin(raiz));
  double distancia = (double)R_tierra * (double)ang_central;
  //printf("%f,%f,%f,%f;%f\t", lat1,lon1,lat2,lon2,distancia);
  return distancia;
}
int* buscar_nueva_posicion_n(int x, int y, double* lista_normal){
  int rand_id_x, rand_id_y;
  double dx, dy;
  int* nueva_pos = malloc(2*sizeof(int));

  do {
    rand_id_x = (int)rand_rango(0,n_puntos_dn);
    rand_id_y = (int)rand_rango(0,n_puntos_dn);
    dx = lista_normal[rand_id_x];
    dy = lista_normal[rand_id_y];

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
      nueva_pos[1] = n_filas - ((int)(y+dy))%n_filas - 1;
      if(nueva_pos[0] >= n_columnas/2.0)
        nueva_pos[0] -= (int)(n_columnas/2);
      else
        nueva_pos[0] += (int)(n_columnas/2);
    }
    else if ((int)(y+dy) < 0){
      nueva_pos[1] = (-((int)(y+dy)))%n_filas;
      if(nueva_pos[0] >= n_columnas/2.0)
        nueva_pos[0] -= (int)(n_columnas/2);
      else
        nueva_pos[0] += (int)(n_columnas/2);
    }
    else {
      nueva_pos[1] = (int)(y+dy);
    }
  } while(x==nueva_pos[0] && y==nueva_pos[1]);

  return nueva_pos;
}
int* buscar_nueva_posicion_u(int x, int y, double radio){
  double dx, dy, random;
  int* nueva_pos = malloc(2*sizeof(int));

  do {
    dx = rand_rango(-radio,2.0*radio);
    dy = sqrt(radio*radio - dx*dx);
    random = rand_rango(0,1);
    if(random > 0.5)
      dy *= -1;
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
      nueva_pos[1] = n_filas - ((int)(y+dy))%n_filas - 1;
      if(nueva_pos[0] >= n_columnas/2.0)
        nueva_pos[0] -= (int)(n_columnas/2);
      else
        nueva_pos[0] += (int)(n_columnas/2);
    }
    else if ((int)(y+dy) < 0){
      nueva_pos[1] = (-((int)(y+dy)))%n_filas;
      if(nueva_pos[0] >= n_columnas/2.0)
        nueva_pos[0] -= (int)(n_columnas/2);
      else
        nueva_pos[0] += (int)(n_columnas/2);
    }
    else {
      nueva_pos[1] = (int)(y+dy);
    }
  } while(x==nueva_pos[0] && y==nueva_pos[1]);

  return nueva_pos;
}
void estimar_radio(int x,int y,double** mapa,int n,double** d_max){
  int radio, rep, parar = 0;
  double dist, max_dist = 0.0;
  for(radio = 1; radio < radio_max_u; radio++){
    for(rep = 0; rep < pow(1.15,radio*n); rep++){
      int *nueva_pos = buscar_nueva_posicion_u(x,y,(double)radio);
      dist = distancia_ortodromica(x, y, nueva_pos[0], nueva_pos[1]);
      if(dist > max_dist)
        max_dist = dist;
      if(mapa[nueva_pos[1]][nueva_pos[0]]){
        parar = 1;
        break;
      }
      free(nueva_pos);
    }
    if(parar)
      break;
    if(max_dist > d_max[y][x])
      d_max[y][x] = max_dist;
    //printf("%f\t", max_dist);
  }
}
void monte_carlo(double** mapa, double** d_max, double* lista_normal){
  int x, y, rand_id_x, rand_id_y, *nueva_pos, i;
  double dx, dy, dist, alpha, beta;
  do {
    x = (int)rand_rango(0,n_columnas);
    y = (int)rand_rango(0,n_filas);
  } while(mapa[y][x] == 1);
  estimar_radio(x,y,mapa,exp_est_rad,d_max);
  for(i = 1; i<n_puntos_MC; i++){
    //printf("Iteracion %d de %d...\n",i,n_puntos_MC-1);
    do {
      nueva_pos = buscar_nueva_posicion_n(x,y,lista_normal);
    } while(mapa[nueva_pos[1]][nueva_pos[0]] == 1);
    estimar_radio(nueva_pos[0],nueva_pos[1],mapa,exp_est_rad,d_max);
    if(d_max[y][x] == 0.0){
      if(d_max[nueva_pos[1]][nueva_pos[0]]){
        alpha = 0.5;
      }
      else{
        alpha = 1.0;
      }
    }
    else{
      alpha = d_max[nueva_pos[1]][nueva_pos[0]]/d_max[y][x];
    }
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
}
void promedio_mc(double **mapa, double **d_max, double *lista_normal){
  int i;
  for(i = 0; i<n_iteraciones; i++){
    //printf("Iteracion %d de %d...\n",i+1,n_iteraciones);
    monte_carlo(mapa, d_max, lista_normal);
  }
}
int* encontrar_maximo(double** distancia_maxima){
  int i, j;
  int *indices = malloc(2*sizeof(int));
  double maximo;
  maximo = distancia_maxima[0][0];
  for(i = 0 ; i < n_filas ; i++)
    for(j = 0 ; j < n_columnas; j++)
      if(distancia_maxima[i][j] > maximo){
        indices[0] = j;
        indices[1] = i;
        maximo = distancia_maxima[i][j];
      }
  return indices;
}
void guardar_maximo(int* max_id, double dist, char* nombre_archivo){
  FILE *archivo = fopen(nombre_archivo, "w");
  fprintf(archivo, "%d,%d,%f", max_id[0],max_id[1],dist);
	fclose(archivo);
}
void liberar_matriz(double** matriz){
    int i;
    for(i = 0 ; i < n_filas ; i++)
      free(matriz[i]);
    free(matriz);
}
