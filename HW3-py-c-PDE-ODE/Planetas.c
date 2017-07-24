/* Se incluyen las librerias necesarias */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Se definen constantes globales necesarias */
#define n_puntos 30000
#define t_sim 3000000
#define n_temp 3
#define n_astros 10
#define n_dim 3
#define runge_kutta_n 4
#define n_columnas 8
const float G = 1.98529e-29;

/*	Se define una estructura para almacenar la informacion de los planetas o el sol. Las matrices posicion y velocidad unicamente
	funcionan como matrices de almacenamiento, mientras las otras dos son las que permiten calcular la evolucion del sistema.	*/
typedef struct{
	char 	nombre[15];
	float	masa;
   	float	posicion[n_dim][n_puntos];
   	float	velocidad[n_dim][n_puntos];
   	float	posicion_temp[n_dim][n_temp];
   	float	velocidad_temp[n_dim][n_temp];
} Astro;

/*	Se define una estructura que contiene a todos los astros del sistema solar, donde cada astro es una instancia de la estructura
	definida anteriormente. Adicionalmente, la estructura contiene 3 matrices que almacenan los vectores de posicion, velocidad y
	aceleracion instantaneos de todos los astros. */
typedef struct{
	Astro astros[n_astros];
	float posicion[n_dim][n_astros];
	float velocidad[n_dim][n_astros];
	float aceleracion[n_dim][n_astros];
} SistemaSolar;

/*	Explicaciones y encabezados de las funciones:
	1. obtener_columna		-	Funcion que recibe un String que contiene datos separados por comas y un numero que
	 							indica cual de estos datos se desea de acuerdo a su posicion.
	2. inicializar_sistema	-	Funcion que lee el archivo con los datos de los astros y los guarda en la instancia
								de la estructura sistema solar.
	3. calculo_velocidad	-	Funcion que obtiene los vectores de velocidad de cada uno de los astros en el instante
								de tiempo especificado y los guarda en la matriz de velocidad del sistema solar.
	4. calculo_aceleracion	-	Funcion que obtiene los vectores de posicion de cada uno de los astros en el instante
								de tiempo especificado y los guarda en la matriz de posicion del sistema solar. A partir
								de esta matriz se calculan los vectores de aceleracion y se guardan en la matriz de
								aceleracion.
	5. leap_frog			-	Funcion que calcula la evolucion del sistema solar por medio del metodo de Euler para el
								primer instante de tiempo y el metodo leap frog para el resto. Esta funcion actualiza
								iterativamente las matrices temporales de los astros y cada t_sim/n_puntos unidades de
								tiempo almacena los resultados en las matrices permanentes de los astros.
	6. exportar_trayectoria	-	Funcion que guarda los datos almacenados en las matrices de posicion de los astros en un
								archivo .csv separado con comas, donde cada columna corresponde a las posiciones de un
								astro, de tal manera que cada 3 filas corresponden a uno de los vectores. */
char* obtener_columna(char* linea, int columna);
void inicializar_sistema(SistemaSolar* apuntador_sisSolar, char* nombre_archivo);
void calculo_velocidad(SistemaSolar* apuntador_sisSolar, int t);
void calculo_aceleracion(SistemaSolar* apuntador_sisSolar, int t);
void leap_frog(SistemaSolar* apuntador_sisSolar, float dt);
void exportar_trayectoria(SistemaSolar* apuntador_sisSolar, char* nombre_archivo);

int main(){
	char nombre_archivo_lectura[30] = "coordinates.csv";
	char nombre_archivo_escritura[30] = "trayectorias.csv";
	float dt = 1e-4;
	SistemaSolar sisSolar;

	inicializar_sistema(&sisSolar, nombre_archivo_lectura);
    leap_frog(&sisSolar, dt);
    exportar_trayectoria(&sisSolar, nombre_archivo_escritura);
    printf("Proceso terminado.\n");

    return 0;
}
char* obtener_columna(char* linea, int columna){
    //strtok divide al string linea y retorna el puntero del primer substring
    char* token = strtok(linea, ",");
    while(columna){
    	token = strtok(NULL, ",\n");
    	columna--;
    }
    return token;
}
void inicializar_sistema(SistemaSolar* apuntador_sisSolar, char* nombre_archivo){
	FILE *archivo = fopen(nombre_archivo, "r");	void
	int max_long = 1024;
	char linea[max_long];
	int astro_id = 0;

	printf("Inicializando el sistema solar...\n");
	while (fgets(linea, max_long, archivo)){
        for(int columna = 0; columna < n_columnas; columna++){
        	//strdup copia al string linea y lo guarda en memoria, retornando la ubicacion de la copia
        	char* copia_linea = strdup(linea);
		    char* dato = obtener_columna(copia_linea, columna);
		    switch (columna){
		    	case 0: strcpy((*apuntador_sisSolar).astros[astro_id].nombre, dato); break;
		    	case 1: (*apuntador_sisSolar).astros[astro_id].masa = strtod(dato,NULL); break;
		    	case 2:
		    	case 3:
		    	case 4: (*apuntador_sisSolar).astros[astro_id].posicion[columna-2][0] = strtod(dato,NULL);
		    		(*apuntador_sisSolar).astros[astro_id].posicion_temp[columna-2][0] = strtod(dato,NULL); break;
		    	case 5:
		    	case 6:
		    	case 7: (*apuntador_sisSolar).astros[astro_id].velocidad[columna-5][0] = strtod(dato,NULL);
		    		(*apuntador_sisSolar).astros[astro_id].velocidad_temp[columna-5][0] = strtod(dato,NULL); break;
		    }
		    free(copia_linea);
    	}
    	astro_id++;
    }
    fclose(archivo);
}
void calculo_velocidad(SistemaSolar* apuntador_sisSolar, int t){
	for(int i=0;i<n_dim;i++)
		for(int j=0;j<n_astros;j++)
			(*apuntador_sisSolar).velocidad[i][j] = (*apuntador_sisSolar).astros[j].velocidad_temp[i][t];
}
void calculo_aceleracion(SistemaSolar* apuntador_sisSolar, int t){
	for(int i=0;i<n_dim;i++)
		for(int j=0;j<n_astros;j++){
			(*apuntador_sisSolar).posicion[i][j] = (*apuntador_sisSolar).astros[j].posicion_temp[i][t];
			(*apuntador_sisSolar).aceleracion[i][j] = 0.0;
		}
	for(int j=0;j<n_astros;j++)
		for(int k=0;k<n_astros;k++)
			if(j!=k){
				float r_jk[n_dim] = {(*apuntador_sisSolar).posicion[0][k]-(*apuntador_sisSolar).posicion[0][j],
									(*apuntador_sisSolar).posicion[1][k]-(*apuntador_sisSolar).posicion[1][j],
									(*apuntador_sisSolar).posicion[2][k]-(*apuntador_sisSolar).posicion[2][j]};
				float d = sqrt(r_jk[0]*r_jk[0] + r_jk[1]*r_jk[1] + r_jk[2]*r_jk[2]);
				float c = G*(*apuntador_sisSolar).astros[k].masa/(d*d*d);
				for(int h=0;h<n_dim;h++)
					(*apuntador_sisSolar).aceleracion[h][j] += c*r_jk[h];
			}
}
void leap_frog(SistemaSolar* apuntador_sisSolar, float dt){
	int t, tmod0, tmod1, tmod2, t_guardado = 0, paso = t_sim/n_puntos;

	// Se calcula la posicion y velocidad en el instante 1, por medio del metodo de Euler
	calculo_velocidad(apuntador_sisSolar, 0);
	calculo_aceleracion(apuntador_sisSolar, 0);
	for(int i=0;i<n_astros;i++)
		for(int h=0;h<n_dim;h++){
			(*apuntador_sisSolar).astros[i].posicion_temp[h][1] =
				(*apuntador_sisSolar).astros[i].posicion_temp[h][0] + dt*(*apuntador_sisSolar).velocidad[h][i];
			(*apuntador_sisSolar).astros[i].velocidad_temp[h][1] =
				(*apuntador_sisSolar).astros[i].velocidad_temp[h][0] + dt*(*apuntador_sisSolar).aceleracion[h][i];
			}

	printf("Calculando trayectorias...\n");
	for(t=2;t<t_sim;t++){
		tmod0 = (t-2) % n_temp;
		tmod1 = (t-1) % n_temp;
		tmod2 = t % n_temp;
		if(t % paso == 0)
			t_guardado++;

		calculo_velocidad(apuntador_sisSolar, tmod1);
		calculo_aceleracion(apuntador_sisSolar, tmod1);

		for(int i=0;i<n_astros;i++)
			for(int h=0;h<n_dim;h++){
				(*apuntador_sisSolar).astros[i].posicion_temp[h][tmod2] =
					(*apuntador_sisSolar).astros[i].posicion_temp[h][tmod0] + 2*dt*(*apuntador_sisSolar).velocidad[h][i];
				(*apuntador_sisSolar).astros[i].velocidad_temp[h][tmod2] =
					(*apuntador_sisSolar).astros[i].velocidad_temp[h][tmod0] +  2*dt*(*apuntador_sisSolar).aceleracion[h][i];
				if(t % paso == 0){
					(*apuntador_sisSolar).astros[i].posicion[h][t_guardado] = (*apuntador_sisSolar).astros[i].posicion_temp[h][tmod2];
					(*apuntador_sisSolar).astros[i].velocidad[h][t_guardado] = (*apuntador_sisSolar).astros[i].velocidad_temp[h][tmod2];
				}
			}
	}
}
void exportar_trayectoria(SistemaSolar* apuntador_sisSolar, char* nombre_archivo){
	printf("Exportando trayectorias...\n");
	FILE *archivo = fopen(nombre_archivo, "w");
	for(int astro_id=0;astro_id<n_astros-1;astro_id++)
		fprintf(archivo, "%s,", (*apuntador_sisSolar).astros[astro_id].nombre);
	fprintf(archivo, "%s\n", (*apuntador_sisSolar).astros[n_astros-1].nombre);
	for(int t=0;t<n_puntos;t++)
		for(int h=0;h<n_dim;h++){
			for(int astro_id=0;astro_id<n_astros-1;astro_id++)
				fprintf(archivo, "%f,", (*apuntador_sisSolar).astros[astro_id].posicion[h][t]);
			fprintf(archivo, "%f\n", (*apuntador_sisSolar).astros[n_astros-1].posicion[h][t]);
		}
	fclose(archivo);
}
