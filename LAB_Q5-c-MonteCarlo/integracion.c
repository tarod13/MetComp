#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define lim_inf 0.0
#define lim_sup 1.0
#define f_min   0.0
#define f_max   1.0
#define n_MC    10000
#define n_int   10000

#define rand_rango(v_inf,v_sup)  v_inf + (double)rand()/((double)RAND_MAX+1.0)*(v_sup-v_inf)

double funcion(double x);
double integracion();
double MonteCarlo();
void guardar_resultado(char* nombre, double integral);

int main(){
  char nombre_archivo[20] = "resultados.txt";
  srand(time(NULL));
  double integral = MonteCarlo();
  printf("%f\n", integral);
  guardar_resultado(nombre_archivo, integral);
  return 0;
}
double funcion(double x){
  double f = exp(-x);
  return f;
}
double integracion(){
  int i, dim = 2, puntos_validos = 0;
  double x, y, integral;
  double area = (double)(f_max-f_min)*(double)(lim_sup-lim_inf);
  for(i = 0; i < n_int; i++){
    x = (double)rand_rango(lim_inf,lim_sup);
    y = (double)rand_rango(f_max,f_min);
    if(y < funcion(x))
      puntos_validos++;
  }
  integral = (double)puntos_validos/(double)n_int*area;
  return integral;
}
double MonteCarlo(){
  int i;
  double integral = 0.0;
  for(i = 0; i < n_MC; i++){
    printf("\rIteracion %d de %d.",i+1,n_MC);
    integral += (double)integracion()/(double)(n_MC);
  }
  printf("\n");
  return integral;
}
void guardar_resultado(char* nombre, double integral){
  FILE* archivo = fopen(nombre,"w");
  fprintf(archivo, "El valor de la integral es: %f", integral);
  fclose(archivo);
}
