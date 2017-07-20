/* Se incluyen las librerias necesarias */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Se definen constantes globales necesarias */
#define n_puntos 2000
#define t_sim 300000
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


char* obtener_columna(char* linea, int columna);
void inicializar_sistema(SistemaSolar* apuntador_sisSolar, char* nombre_archivo);

/*
void calculo_velocidad(SistemaSolar* apuntador_sisSolar, float** dv, int t){
	for(int i=0;i<n_dim;i++)
		for(int j=0;j<n_astros;j++) 	
			(*apuntador_sisSolar).velocidad[i][j] = (*apuntador_sisSolar).astros[j].velocidad_temp[i][t] + dv[i][j];	
}

void calculo_aceleracion(SistemaSolar* apuntador_sisSolar, float** dx, int t){	
	for(int i=0;i<n_dim;i++)
		for(int j=0;j<n_astros;j++){
			(*apuntador_sisSolar).posicion[i][j] = (*apuntador_sisSolar).astros[j].posicion_temp[i][t] + dx[i][j];
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

void runge_kutta_4(SistemaSolar* apuntador_sisSolar, float dt){
	int t, tmod0, tmod1, t_guardado = 0, paso = t_sim/n_puntos;
	float** dx = (float **)malloc(n_dim * sizeof(float*));
	for(int m=0;m<n_dim;m++) dx[m] = (float *)malloc(n_astros * sizeof(float));
	float** dv = (float **)malloc(n_dim * sizeof(float*));
	for(int m=0;m<n_dim;m++) dv[m] = (float *)malloc(n_astros * sizeof(float));
	float k1[runge_kutta_n][n_dim][n_astros], k2[runge_kutta_n][n_dim][n_astros];
	
	printf("Calculando trayectorias..\n");
	for(t=1;t<t_sim;t++){
		tmod0 = (t-1) % n_temp;
		tmod1 = t % n_temp;
		if(t % paso == 0)	
			t_guardado++;	
				
		for(int i=0;i<n_astros;i++)
			for(int h=0;h<n_dim;h++){			
				dx[h][i] = 0.0;
				dv[h][i] = 0.0;
			}
			
		for(int m=0;m<runge_kutta_n;m++){
			calculo_velocidad(apuntador_sisSolar, dv, tmod0);
			calculo_aceleracion(apuntador_sisSolar, dx, tmod0);
			float coef;
			switch(m){
				case 0: coef = 1.0/2.0; break;
				case 1: coef = 1.0/2.0; break;
				case 2: coef = 1.0; break;
				case 3: coef = 0.0; break;
			}			 
			for(int i=0;i<n_astros;i++)
				for(int h=0;h<n_dim;h++){
					k1[m][h][i] = dt*(*apuntador_sisSolar).velocidad[h][i];
					k2[m][h][i] = dt*(*apuntador_sisSolar).aceleracion[h][i];
					dx[h][i] = coef*k1[m][h][i];
					dv[h][i] = coef*k2[m][h][i]; 
				}
		} 
		
		for(int i=0;i<n_astros;i++)
			for(int h=0;h<n_dim;h++){
				(*apuntador_sisSolar).astros[i].posicion_temp[h][tmod1] = 
					(*apuntador_sisSolar).astros[i].posicion_temp[h][tmod0] + 
					(k1[0][h][i]+2*k1[1][h][i]+2*k1[2][h][i]+k1[3][h][i])/6.0;
				(*apuntador_sisSolar).astros[i].velocidad_temp[h][tmod1] = 
					(*apuntador_sisSolar).astros[i].velocidad_temp[h][tmod0] + 
					(k2[0][h][i]+2*k2[1][h][i]+2*k2[2][h][i]+k1[3][h][i])/6.0;
				if(t % paso == 0){
					(*apuntador_sisSolar).astros[i].posicion[h][t_guardado] = 
						(*apuntador_sisSolar).astros[i].posicion_temp[h][tmod0] + 
						(k1[0][h][i]+2*k1[1][h][i]+2*k1[2][h][i]+k1[3][h][i])/6.0;
					(*apuntador_sisSolar).astros[i].velocidad[h][t_guardado] = 
						(*apuntador_sisSolar).astros[i].velocidad_temp[h][tmod0] + 
						(k2[0][h][i]+2*k2[1][h][i]+2*k2[2][h][i]+k1[3][h][i])/6.0;					
				}
			}									
	}
	free(dx);
	free(dv);	
}
*/

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
	
	// Se calcula la posicion y velocidad en el instante 1, por medio del metodo de euler 
	calculo_velocidad(apuntador_sisSolar, 0);
	calculo_aceleracion(apuntador_sisSolar, 0);	
	for(int i=0;i<n_astros;i++)
		for(int h=0;h<n_dim;h++){
			(*apuntador_sisSolar).astros[i].posicion_temp[h][1] = 
				(*apuntador_sisSolar).astros[i].posicion_temp[h][0] + dt*(*apuntador_sisSolar).velocidad[h][i];
			(*apuntador_sisSolar).astros[i].velocidad_temp[h][1] = 
				(*apuntador_sisSolar).astros[i].velocidad_temp[h][0] + dt*(*apuntador_sisSolar).aceleracion[h][i];					
			}		
	
	printf("Calculando trayectorias..\n");
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
	printf("Exportando trayectorias..\n");
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

int main(){	
	char nombre_archivo_lectura[30] = "coordinates.csv";
	SistemaSolar sisSolar;
	//float dt_rk = 1e-7;
	float dt_lf = 1e-3;
	char nombre_archivo_escritura[30] = "trayectorias.csv";
	
	inicializar_sistema(&sisSolar, nombre_archivo_lectura);
    //runge_kutta_4(&sisSolar, dt_rk);
    leap_frog(&sisSolar, dt_lf);
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
	FILE *archivo = fopen(nombre_archivo, "r");	
	int max_long = 1024;
	char linea[max_long];
	int astro_id = 0;
	
	printf("Inicializando el sistema solar..\n");
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

