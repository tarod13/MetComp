/* Se importan librerias necesarias*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* Se definen constantes*/
#define n_filas         500
#define n_columnas      744
#define n_columnas_f    0
#define n_columnas_o    744
#define n_puntos_dn     100000
#define n_puntos_MC     2000
#define n_iteraciones   1
#define ORIGINAL        1
#define MODIFICADA      0
#define PI              3.14159265359
#define sigma           30
#define radio_max_n     10
#define radio_max_u     8000
#define R_tierra        6371

/* Se definen macros*/
#define ind_a_lon(x) ((double)(x)/(double)(n_columnas)-0.5)*360.0
#define ind_a_lat(y) -((double)(y)/(double)(n_filas)-0.5)*180.0
#define lon_a_ind(x) (int)(((double)(x)/360.0+0.5)*(double)(n_columnas))
#define lat_a_ind(y) (int)((-(double)(y)/180.0+0.5)*(double)(n_filas))
#define rad_a_grad(x) (double)x*180.0/(double)(PI)
#define grad_a_rad(x) (double)x*(double)(PI)/180.0
#define rand_rango(vmin, rango) vmin+((double)rand()/((double)RAND_MAX + 1))*rango
#define semivseno(a, b) (double)sin((double)(a-b)/2.0)*(double)sin((double)(a-b)/2.0)

/*Encabezados de las funciones*/
double** crear_matriz(double valor, int original);
void lectura_archivo(char *nombre_archivo, double** mapa);
double* crear_lista_normal();
double distancia_ortodromica(int x1, int y1, int x2, int y2);
int* buscar_nueva_posicion_n(int x,int y,double* lista_normal);
int* buscar_nueva_posicion_u(int x,int y,double radio, double n, int n_dy);
void estimar_radio(int x, int y, double** mapa, double** d_max, double** d_min);
void monte_carlo(double** mapa, double** d_max, double** d_min, double* lista_normal);
int* encontrar_maximo(double **d_min);
void guardar_maximo(int* max_id, double dist, char* nombre_archivo);
void liberar_matriz(double** matriz);

/*Funcion principal*/
int main() {
  srand(time(NULL));

  char nombre_archivo_lectura[20] = "map_data.txt";
  char nombre_archivo_escritura[20] = "pmax.txt";
  char nombre_archivo_escritura2[20] = "map.txt";

  double** mapa = crear_matriz(0.0,MODIFICADA);
  double** d_max = crear_matriz(PI*R_tierra,MODIFICADA);
  double** d_min = crear_matriz(0.0,MODIFICADA);
  double* lista_normal = crear_lista_normal();

  lectura_archivo(nombre_archivo_lectura, mapa);
  printf("Calculando punto mas lejano...\n");
  monte_carlo(mapa, d_max, d_min, lista_normal);
  int* id_max = encontrar_maximo(d_min);

  double lon =  ind_a_lon(id_max[0]);
  double lat = ind_a_lat(id_max[1]);
  double dist = d_min[id_max[1]][id_max[0]];
  double dist2 = d_max[id_max[1]][id_max[0]];
  printf("las coordenadas del punto mas alejado son: %f, %f; %f,%f\n",lon,lat,dist,dist2);
  guardar_maximo(id_max, dist, nombre_archivo_escritura);

  liberar_matriz(mapa);
  liberar_matriz(d_max);
  liberar_matriz(d_min);
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
      while(n_columna<n_columnas_o-1){
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
  double lon1, lat1, lon2, lat2, semivseno_lat, semivseno_lon, raiz, mul_cos;
  double ang_central, distancia;
  lon1 = grad_a_rad(ind_a_lon(x1));
  lat1 = grad_a_rad(ind_a_lat(y1));
  lon2 = grad_a_rad(ind_a_lon(x2));
  lat2 = grad_a_rad(ind_a_lat(y2));
  semivseno_lat = semivseno(lat1,lat2);
  semivseno_lon = semivseno(lon1,lon2);
  mul_cos = (double)cos(lat1)*(double)cos(lat2);
  raiz = (double)sqrt(semivseno_lat + mul_cos*semivseno_lon);
  ang_central = 2.0*(double)(asin(raiz));
  distancia = (double)(R_tierra) * ang_central;

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
      nueva_pos[0] = n_columnas - (-(int)(x+dx))%n_columnas;
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
int* buscar_nueva_posicion_u(int x, int y, double radio, double n, int n_dy){
  int invertir = 0;
  double dx, random, delta, lon1, lat1, lat2, semivseno_lat, semivseno_rad;
  double mul_cos, dx_abs_rad;
  int* nueva_pos = malloc(3*sizeof(int));

  delta = rad_a_grad(radio/(double)(R_tierra))*(2.0*(double)n_dy/n-1.0);
  nueva_pos[0] = 0;
  nueva_pos[1] = lat_a_ind((double)(ind_a_lat(y))+delta);
  nueva_pos[2] = 0;
  //printf("%d, %f, %d\n", y,ind_a_lat(y)+delta,nueva_pos[1]);
  if(nueva_pos[1] >= n_filas){
    nueva_pos[1] = n_filas - nueva_pos[1]%n_filas - 1;
    invertir = 1;
  }
  else if (nueva_pos[1] < 0){
    nueva_pos[1] = (-nueva_pos[1])%n_filas;
    invertir = 1;
  }

  lon1 = grad_a_rad(ind_a_lon(x));
  lat1 = grad_a_rad(ind_a_lat(y));
  lat2 = grad_a_rad(ind_a_lat(nueva_pos[1]));
  semivseno_lat = semivseno(lat1, lat2);
  semivseno_rad = semivseno((double)(radio)/(double)(R_tierra), 0.0);
  mul_cos = (double)cos(lat1)*(double)cos(lat2);
  dx_abs_rad = 2.0*asin(sqrt((semivseno_rad-semivseno_lat)/mul_cos));
  dx = (double)(rad_a_grad(dx_abs_rad));
  nueva_pos[0] = lon_a_ind((double)(ind_a_lon(x)+dx));
  nueva_pos[2] = lon_a_ind((double)(ind_a_lon(x)-dx));
  if(nueva_pos[0] >= n_columnas || nueva_pos[0] <= -n_columnas)
    nueva_pos[0] = x;
  if(nueva_pos[2] >= n_columnas || nueva_pos[2] <= -n_columnas)
    nueva_pos[2] = x;

  if(nueva_pos[0] >= n_columnas){
    nueva_pos[0] = nueva_pos[0]%n_columnas;
  }
  else if (nueva_pos[0] < 0){
    nueva_pos[0] = n_columnas + nueva_pos[0];
  }
  if(nueva_pos[2] >= n_columnas){
    nueva_pos[2] = nueva_pos[2]%n_columnas;
  }
  else if (nueva_pos[2] < 0){
    nueva_pos[2] = n_columnas + nueva_pos[2];
  }

  if(invertir){
    if(nueva_pos[0] >= n_columnas/2.0)
      nueva_pos[0] -= (int)(n_columnas/2);
    else
      nueva_pos[0] += (int)(n_columnas/2);

    if(nueva_pos[2] >= n_columnas/2.0)
      nueva_pos[2] -= (int)(n_columnas/2);
    else
      nueva_pos[2] += (int)(n_columnas/2);
  }

  return nueva_pos;
}
void estimar_radio(int x,int y,double** mapa,double** d_max, double** d_min){
  int rep;
  double radio, dist, max_dist = 0.0;
  for(radio = 20.0; radio < radio_max_u && radio < d_max[y][x]; radio += 20.0){
    for(rep = 0; rep < radio/10.0 ; rep++){
      int *nueva_pos = buscar_nueva_posicion_u(x,y,radio,radio/10.0,rep);
      dist = distancia_ortodromica(x, y, nueva_pos[0], nueva_pos[1]);

      if(mapa[nueva_pos[1]][nueva_pos[0]] == 1 || mapa[nueva_pos[1]][nueva_pos[2]] == 1){
        if(dist < d_max[y][x]){
          d_max[y][x] = dist;
          if(dist < d_min[y][x])
            d_min[y][x] = dist;
        }
        break;
      }
      else{
        if(dist > d_min[y][x] && dist < d_max[y][x])
          d_min[y][x] = dist;
      }
      free(nueva_pos);
    }
    //printf("%f\t", max_dist);
  }
}
void monte_carlo(double** mapa, double** d_max, double** d_min, double* lista_normal){
  int x, y, rand_id_x, rand_id_y, *nueva_pos, i;
  double dx, dy, dist, alpha, beta;
  do {
    x = (int)rand_rango(0,n_columnas);
    y = (int)rand_rango(0,n_filas);
  } while(mapa[y][x] == 1);
  printf("Iteracion 1 de %d...",n_puntos_MC);
  estimar_radio(x,y,mapa,d_max,d_min);
  for(i = 1; i<n_puntos_MC; i++){
    printf("\rIteracion %d de %d...",i+1,n_puntos_MC);
    do {
      nueva_pos = buscar_nueva_posicion_n(x,y,lista_normal);
    } while(mapa[nueva_pos[1]][nueva_pos[0]] == 1);
    estimar_radio(nueva_pos[0],nueva_pos[1],mapa,d_max,d_min);
    if(d_min[y][x] == 0.0){
      if(d_min[nueva_pos[1]][nueva_pos[0]]){
        alpha = 0.5;
      }
      else{
        alpha = 1.0;
      }
    }
    else{
      alpha = d_min[nueva_pos[1]][nueva_pos[0]]/d_min[y][x];
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
  printf("\n");
}
int* encontrar_maximo(double** d_min){
  int i, j;
  int *indices = malloc(2*sizeof(int));
  double maximo;
  maximo = d_min[0][0];
  for(i = 0 ; i < n_filas ; i++)
    for(j = 0 ; j < n_columnas; j++)
      if(d_min[i][j] >= maximo){
        indices[0] = j;
        indices[1] = i;
        maximo = d_min[i][j];
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
